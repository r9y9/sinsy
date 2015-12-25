/* ----------------------------------------------------------------- */
/*           The HMM-Based Singing Voice Synthesis System "Sinsy"    */
/*           developed by Sinsy Working Group                        */
/*           http://sinsy.sourceforge.net/                           */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2015  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the Sinsy working group nor the names of    */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#include <sstream>
#include <algorithm>
#include <stack>
#include <stdexcept>
#include "XmlParser.h"
#include "InputFile.h"
#include "util_string.h"
#include "util_log.h"

namespace sinsy
{

namespace
{

/*!
 get char
 */
char getChar(IReadableStream& stream) throw (StreamException)
{
   char c = '\0';
   int result = stream.read(static_cast<void*>(&c), 1);
   if (0 == result) {
      throw StreamException("getChar() end of file");
   } else if (result < 0) {
      throw StreamException("getChar() error");
   }
   return c;
}

/*!
 get next valid char
 */
char getNextValidChar(IReadableStream& stream) throw (StreamException)
{
   for ( ; ; ) {
      char c(getChar(stream));
      if (!isBlank(c)) {
         return c;
      }
   }
}

/*!
 get next tag

 @param fp   input fp
 @param data buffer to store data up to the next tag
 @param tag  buffer to store the next tag (not contain '<' and '>')
 */
void getNextTag(IReadableStream& stream, std::string& data, std::string& tag) throw (StreamException)
{
   std::ostringstream oss;

   // skip blanks
   char c = getNextValidChar(stream);

   while ('<' != c) {
      oss << c;
      c = getChar(stream);
   }
   data = oss.str();
   // remove blanks from the tail of data
   cutBlanks(data);

   oss.str("");

   // skip blanks
   c = getNextValidChar(stream);

   while ('>' != c) {
      oss << c;
      c = getChar(stream);
   }
   tag = oss.str();
   // remove blanks from the tail of tag
   cutBlanks(tag);
}

/*!
 create xml data
 */
XmlData* createXmlData(const std::string& tag, const std::string& data = "")
{
   size_t idx = findFirstOfBlank(tag);
   if (0 == idx) {
      StreamException("createXmlData() no tag");
   }
   if (std::string::npos == idx) { // no attributres
      return new XmlData(tag);
   }

   XmlData* ret(new XmlData(tag.substr(0, idx)));
   const size_t sz(tag.size());
   while (idx < sz) {
      size_t at(tag.find('=', idx)); // search '='
      if (std::string::npos == at) {
         delete ret;
         throw StreamException("createXmlData() '=' is not exist");
      }
      if (at == idx) {
         delete ret;
         throw StreamException("createXmlData() '=' is at the head of tag");
      }
      std::string key(tag.substr(idx, at - idx)); // cut up to '='
      cutBlanks(key);
      idx = at + 1;
      idx = findFirstNotOfBlank(tag, idx);

      char quotation = '\0';
      // attribute value must start with ' or "
      if (('\'' == tag[idx]) || ('\"' == tag[idx])) {
         quotation = tag[idx];
      } else {
         delete ret;
         throw StreamException("xml attribute value needs \" or \'");
      }
      ++idx;
      size_t start(idx);

      // search last ' or "
      for ( ; ; ) {
         idx = tag.find(quotation, idx);
         if (std::string::npos == idx) {
            delete ret;
            throw StreamException("xml attribute value needs \" or \'");
         }
         // last ' or " following \ should be ignored
         if ((start != idx) || ('\\' != tag[idx - 1])) {
            break;
         } else {
            ++idx;
         }
      }
      std::string value(tag.substr(start, idx - start));
      ret->addAttribute(key, value);
      ++idx;
      idx = findFirstNotOfBlank(tag, idx);
   }
   return ret;
}

};

/*!
 constructor
 */
XmlParser::XmlParser()
{
}

/*!
 destructor
 */
XmlParser::~XmlParser()
{
}

/*!
 read data from stream
 */
XmlData* XmlParser::read(IReadableStream& stream, std::string& encoding) throw (StreamException)
{
   XmlData* topData(NULL);
   std::stack<XmlData*> dataStack;
   std::string data;
   std::string tag;

   try {
      for ( ; ; ) {
         getNextTag(stream, data, tag);

         if ('/' == tag[0]) { // end tag
            tag.erase(0, 1);
            cutBlanks(tag);
            if (tag.empty()) {
               throw StreamException("end tag is empty : </ >");
            }
            if (dataStack.empty()) {
               throw StreamException("start tag is needed before end tag");
            }
            XmlData* xd(dataStack.top());
            if (0 != tag.compare(xd->getTag())) {
               throw StreamException("start tag and end tag are not match");
            }
            xd->setData(data);
            dataStack.pop();
            if (dataStack.empty()) {
               break;
            }

         } else if ('?' == tag[0]) { // processing instruction
            if (0 == tag.compare(0, 4, "?xml")) { // if <?xml ... ?> tag, read character encoding
               tag = tag.substr(1, tag.size() - 2);
               cutBlanks(tag);
               if (tag.empty()) {
                  throw StreamException("start tag is empty : < />");
               }
               XmlData* xd(createXmlData(tag));
               std::string enc = xd->getAttribute("encoding");
               if (!enc.empty()) {
                  encoding = enc;
               }
               delete xd;
            }
         } else if ('!' == tag[0]) { // skip
         } else { // start tag
            bool hasEndTag(true);
            if ('/' == tag[tag.size() - 1]) { // start tag which donot need end tag
               tag.erase(tag.size() - 1, 1);
               cutBlanks(tag);
               if (tag.empty()) {
                  throw StreamException("start tag is empty : < />");
               }
               data.clear(); // fail safe
               hasEndTag = false;
            }
            XmlData* xd(createXmlData(tag));
            if (dataStack.empty()) {
               topData = xd;
            } else {
               dataStack.top()->addChild(xd);
            }
            if (hasEndTag) {
               dataStack.push(xd);
            }
         }
      }
   } catch (const std::exception& ex) {
      ERR_MSG("XML parsing error : " << ex.what());
      delete topData;
      throw;
   }
   return topData;
}

};  // namespace sinsy

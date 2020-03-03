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

#ifndef SINSY_XML_DATA_H_
#define SINSY_XML_DATA_H_

#include <vector>
#include <map>
#include <string>
#include "StreamException.h"
#include "IStringable.h"
#include "Beat.h"

namespace sinsy
{

class IReadableStream;

class XmlData : public IStringWritable
{
public:
   typedef std::vector<XmlData*> Children;

   //! constructor
   XmlData(const std::string& t, const std::string& d = "");

   //! copy constructor
   XmlData(const XmlData&);

   //! destructor
   virtual ~XmlData();

   //! get tag
   const std::string& getTag() const;

   //! get data
   const std::string& getData() const;

   //! set data
   void setData(const std::string& str);

   //! add child
   Children::iterator addChild(XmlData* child);

   //! erase child
   XmlData::Children::iterator eraseChild(const Children::iterator& itr);

   //! add attribute
   bool addAttribute(const std::string& k, const std::string& v);

   //! get attribute
   const std::string& getAttribute(const std::string& k) const;

   //! push into stringstream
   std::ostream& toStringStream(std::ostream& os) const;

   //! get begin const iterator of children
   Children::const_iterator childBegin() const;

   //! get end const iterator of children
   Children::const_iterator childEnd() const;

   //! get begin iterator of children
   Children::iterator childBegin();

   //! get end iterator of children
   Children::iterator childEnd();

private:
   //! assignment operator (donot use)
   XmlData& operator=(const XmlData&);

   //! tag
   std::string tag;

   //! data
   std::string data;

   //! children
   Children children;

   typedef std::map<std::string, std::string> Attributes;

   //! attributes
   Attributes attributes;
};

};

#endif // SINSY_XML_DATA_H_

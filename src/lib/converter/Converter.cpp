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

#include <algorithm>
#include <exception>
#include <stdexcept>
#include "Converter.h"
#include "IConvertable.h"
#include "ConfGroup.h"
#include "util_log.h"
#include "util_converter.h"
#include "Deleter.h"

namespace sinsy
{

namespace
{

/*!
 convert resets
*/
void convertRests(IConf::ConvertableList::iterator begin, IConf::ConvertableList::iterator end, const std::string& silStr)
{
   if (begin == end) {
      return;
   }

   IConf::ConvertableList::iterator itr(begin);
   for (; itr != end; ++itr) {
      IConvertable& convertable(**itr);
      if (convertable.isRest() && !convertable.isConverted()) {
         std::vector<PhonemeInfo> phonemes;
         std::string restType(PhonemeInfo::TYPE_SILENT);
         std::string restStr(silStr);

         if (begin != itr) { // not head
            if (!(*(itr - 1))->isRest()) { // prev note is not rest
               restStr = DEFAULT_PAU_STR;
               restType = PhonemeInfo::TYPE_PAUSE;
            }
         }
         if (end - 1 != itr) { // not tail
            if (!(*(itr + 1))->isRest()) { // prev note is not rest
               restStr = DEFAULT_PAU_STR;
               restType = PhonemeInfo::TYPE_PAUSE;
            }
         }
         phonemes.push_back(PhonemeInfo(restType, restStr));
         convertable.addInfo(phonemes);
      }
   }
}
};

/*!
 constructor
*/
Converter::Converter()
{
}

/*!
 destructor
*/
Converter::~Converter()
{
   clear();
}

/*!
 clear
*/
void Converter::clear()
{
   confs.clear();
}

/*!
 set confs to converter
 */
bool Converter::setLanguages(const std::string& languages, const std::string& dirPath)
{
   return confManager.setLanguages(languages, dirPath);
}

/*!
 get sil string
 */
std::string Converter::getSilStr() const
{
   if (!confs.empty()) {
      return confs.front()->getSilStr();
   }
   return DEFAULT_SIL_STR;
}

/*!
 convert
*/
bool Converter::convert(const std::string& enc, IConf::ConvertableList::iterator begin, IConf::ConvertableList::iterator end) const
{
   // convert rests
   convertRests(begin, end, this->getSilStr());

   // convert pitches
   ConfGroup confs;
   confManager.setDefaultConfs(confs);
   IConf::ConvertableList::iterator b(begin);
   while (end != b) {
      IConvertable& bConv(**b);
      if (bConv.isConverted()) {
         ++b;
         continue;
      }

      IConf::ConvertableList::iterator e(b);
      for (++e; end != e; ++e) {
         IConvertable& eConv(**e);
         if (eConv.isConverted()) {
            confs.convert(enc, b, e);
            b = e + 1;
            break;
         }
      }
      if (end == e) {
         confs.convert(enc, b, e);
         b = e;
      }
   }

   return true;
}

};  // namespace sinsy

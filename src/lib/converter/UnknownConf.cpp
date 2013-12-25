/* ----------------------------------------------------------------- */
/*           The HMM-Based Singing Voice Synthesis System "Sinsy"    */
/*           developed by Sinsy Working Group                        */
/*           http://sinsy.sourceforge.net/                           */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2013  Nagoya Institute of Technology          */
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
#include "util_log.h"
#include "UnknownConf.h"
#include "IConvertable.h"

using namespace sinsy;

namespace
{
const std::string SIL_STR = "sil";
const char DISABLE_CHAR = '#';
const char FALSETTO_CHAR = '$';
const std::string LANGUAGE_INFO = "";
};

/*!
 constructor
 */
UnknownConf::UnknownConf()
{
}

/*!
 destructor
*/
UnknownConf::~UnknownConf()
{
}

/*!
 clear
 */
void UnknownConf::clear()
{
}

/*!
 convert lyrics to phonemes

 @param enc encoding
 @param begin start of note list
 @param end last of note list
 @return if success, return true
*/
bool UnknownConf::convert(const std::string& enc, ConvertableList::iterator begin, ConvertableList::iterator end) const
{
   IConf::ConvertableList::iterator itr(begin);
   for (; itr != end; ++itr) {
      IConvertable& convertable(**itr);
      if (!convertable.isConverted()) {
         std::string lyric(convertable.getLyric());

         bool enableFlag = true;
         bool falsettoFlag = false;

         // check disable char
         for (size_t idx(0); ; ) {
            idx = lyric.find(DISABLE_CHAR, idx);
            if (std::string::npos == idx) {
               break;
            }
            lyric.erase(idx, idx + 1);
            enableFlag = false;
         }

         // check falsetto char
         for (size_t idx(0); ; ) {
            idx = lyric.find(FALSETTO_CHAR, idx);
            if (std::string::npos == idx) {
               break;
            }
            lyric.erase(idx, idx + 1);
            falsettoFlag = true;
         }
         if (enableFlag) {
            WARN_MSG("Lyric in unknown language : " << convertable.getLyric());
         }
         std::vector<PhonemeInfo> phonemes;
         phonemes.push_back(PhonemeInfo("", lyric, enableFlag, falsettoFlag));
         convertable.addInfo(phonemes, LANGUAGE_INFO, "");
      }
   }
   return true;
}

/*!
 get sil str

 return edge rest
 */
std::string UnknownConf::getSilStr() const
{
   return SIL_STR;
}

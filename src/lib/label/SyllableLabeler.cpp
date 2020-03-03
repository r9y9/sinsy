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
#include <stdexcept>
#include "SyllableLabeler.h"
#include "PhonemeLabeler.h"
#include "util_log.h"
#include "Deleter.h"

namespace sinsy
{

/*!
 constructor
 */
SyllableLabeler::SyllableLabeler(const std::string& lang) : language(lang), idxInNote(0), numInNote(0)
{
}

/*!
 destructor
 */
SyllableLabeler::~SyllableLabeler()
{
   std::for_each(children.begin(), children.end(), Deleter<PhonemeLabeler>());
}

/*!
 set label
 */
void SyllableLabeler::setLabel(ISyllableLabel& label) const
{
   label.setPhonemeNum(children.size());
   if (numInNote < idxInNote) {
      throw std::runtime_error("SyllableLabeler::setLabel() numInNote < idxInNote");
   }
   label.setPositionInNote(idxInNote, numInNote);
   if (!language.empty()) {
      label.setLanguage(language);
   }
   if (!langDependentInfo.empty()) {
      label.setLangDependentInfo(langDependentInfo);
   }
}

/*!
 add phoneme
 */
void SyllableLabeler::addChild(PhonemeLabeler* l)
{
   children.push_back(l);
}

/*!
 get begin interator of phonemes
 */
SyllableLabeler::List::const_iterator SyllableLabeler::childBegin() const
{
   return children.begin();
}

/*!
 get end interator of phonemes
 */
SyllableLabeler::List::const_iterator SyllableLabeler::childEnd() const
{
   return children.end();
}

/*!
 set index in note
 */
void SyllableLabeler::setIdxInNote(size_t i)
{
   idxInNote = i;
}

/*!
 set number in note
 */
void SyllableLabeler::setNumInNote(size_t n)
{
   numInNote = n;
}

/*!
 set language dependent information
 */
void SyllableLabeler::setInfo(const std::string& str)
{
   langDependentInfo = str;
}

/*!
 set phoneme positions
 */
void SyllableLabeler::setPhonemePositions()
{
   // position
   {
      const List::iterator itrEnd(children.end());
      size_t sz(children.size());
      size_t pos(0);
      for (List::iterator itr(children.begin()); itrEnd != itr; ++itr) {
         (*itr)->setIdxInSyllable(pos);
         ++pos;
         (*itr)->setNumInSyllable(sz);
      }
   }

   // count from prev vowel
   {
      size_t count(0);
      const List::iterator itrEnd(children.end());
      for (List::iterator itr(children.begin()); itrEnd != itr; ++itr) {
         const std::string& type((*itr)->getPhonemeType());
         if (0 == type.compare(PhonemeInfo::TYPE_VOWEL)) {
            count = 1;
         } else {
            if (0 == type.compare(PhonemeInfo::TYPE_CONSONANT)) {
               (*itr)->setCountFromPrevVowel(count);
            }
            if (0 < count) {
               ++count;
            }
         }
      }
   }

   // count from next vowel
   {
      size_t count(0);
      const List::reverse_iterator itrEnd(children.rend());
      for (List::reverse_iterator itr(children.rbegin()); itrEnd != itr; ++itr) {
         const std::string& type((*itr)->getPhonemeType());
         if (0 == type.compare(PhonemeInfo::TYPE_VOWEL)) {
            count = 1;
         } else {
            if (0 == type.compare(PhonemeInfo::TYPE_CONSONANT)) {
               (*itr)->setCountToNextVowel(count);
            }
            if (0 < count) {
               ++count;
            }
         }
      }
   }
}

};  // namespace sinsy

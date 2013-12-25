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

#include "PhonemeLabeler.h"
#include "util_log.h"
#include <stdexcept>

using namespace sinsy;

const std::string PhonemeLabeler::BREATH_PHONEME = "br"; //!< breath

/*!
 constructor
 */
PhonemeLabeler::PhonemeLabeler(const PhonemeInfo& p) : phonemeInfo(p), idxInSyllable(0), numInSyllable(0)
{
}

/*!
 destructor
 */
PhonemeLabeler::~PhonemeLabeler()
{
}

/*!
 set label
 */
void PhonemeLabeler::setLabel(IPhonemeLabel& label, int overwriteEnableFlag) const
{
   label.setType(phonemeInfo.getType());
   label.setName(phonemeInfo.getPhoneme());
   size_t flag = 0;

   if (((0 == overwriteEnableFlag) && phonemeInfo.isEnable()) || (1 == overwriteEnableFlag)) {
      flag |= static_cast<size_t>(1);
   }
   if (phonemeInfo.isFalsetto()) {
      flag |= static_cast<size_t>(2);
   }
   label.setFlag(flag);

   if (numInSyllable < idxInSyllable) {
      throw std::runtime_error("PhonemeLabeler::setLabel() numInSyllable < idxInSyllable");
   }
   label.setPositionInSyllable(idxInSyllable, numInSyllable);
}

/*!
 return which enable or not
 */
bool PhonemeLabeler::isEnable() const
{
   return phonemeInfo.isEnable();
}

/*!
 return which falset or not
 */
bool PhonemeLabeler::isFalsetto() const
{
   return phonemeInfo.isFalsetto();
}

/*!
 set index in syllable
 */
void PhonemeLabeler::setIdxInSyllable(size_t i)
{
   idxInSyllable = i;
}

/*!
 set number in syllable
 */
void PhonemeLabeler::setNumInSyllable(size_t n)
{
   numInSyllable = n;
}

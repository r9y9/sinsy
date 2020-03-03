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

#ifndef SINSY_PHONEME_LABELER_H_
#define SINSY_PHONEME_LABELER_H_

#include <vector>
#include <string>
#include "IPhonemeLabel.h"
#include "PhonemeInfo.h"

namespace sinsy
{

class PhonemeLabeler
{
public:
   static const std::string BREATH_PHONEME;

   //! constructor
   explicit PhonemeLabeler(const PhonemeInfo&);

   //! destructor
   virtual ~PhonemeLabeler();

   //! set label
   virtual void setLabel(IPhonemeLabel&, int overwriteEnableFlag = 0) const;

   //! get score flag
   ScoreFlag getScoreFlag() const;

   //! set index in syllable
   void setIdxInSyllable(size_t i);

   //! set number in syllable
   void setNumInSyllable(size_t n);

   //! get phoneme type
   const std::string& getPhonemeType() const;

   //! set count from previous vowel
   void setCountFromPrevVowel(size_t count);

   //! set count to next vowel
   void setCountToNextVowel(size_t count);

private:
   //! copy constructor (donot use)
   PhonemeLabeler(const PhonemeLabeler&);

   //! assignment operator (donot use)
   PhonemeLabeler& operator=(const PhonemeLabeler&);

   // phoneme information
   PhonemeInfo phonemeInfo;

   //! index in syllable
   size_t idxInSyllable;

   //! number in syllable
   size_t numInSyllable;

   //! count from prev vowel
   size_t countFromPrevVowel;

   //! count to next vowel
   size_t countToNextVowel;
};

};

#endif // SINSY_PHONEME_LABELER_H_

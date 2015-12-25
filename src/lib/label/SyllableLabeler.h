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

#ifndef SINSY_SYLLABLE_LABELER_H_
#define SINSY_SYLLABLE_LABELER_H_

#include <vector>
#include "LabelPosition.h"
#include "PhonemeLabeler.h"
#include "ISyllableLabel.h"

namespace sinsy
{

class SyllableLabeler
{
public:
   typedef std::vector<PhonemeLabeler*> List;

   //! constructor
   explicit SyllableLabeler(const std::string& lang);

   //! destructor
   virtual ~SyllableLabeler();

   //! set label
   virtual void setLabel(ISyllableLabel&) const;

   //! add phoneme
   void addChild(PhonemeLabeler* l);

   //! get begin iterator of phonemes
   List::const_iterator childBegin() const;

   //! get end interator of phonemes
   List::const_iterator childEnd() const;

   //! set language dependent information
   void setInfo(const std::string& str);

   //! set index in note
   void setIdxInNote(size_t i);

   //! set number in note
   void setNumInNote(size_t n);

   //! set phoneme positions
   void setPhonemePositions();

private:
   //! copy constructor (donot use)
   SyllableLabeler(const SyllableLabeler&);

   //! assignment operator (donot use)
   SyllableLabeler& operator=(const SyllableLabeler&);

   //! children (phonemes)
   List children;

   //! language
   std::string language;

   //! index in note
   size_t idxInNote;

   //! number in note
   size_t numInNote;

   //! language dependent information
   std::string langDependentInfo;
};

};

#endif // SINSY_SYLLABLE_LABELER_H_

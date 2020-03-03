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

#ifndef SINSY_LABEL_MEASURE_H_
#define SINSY_LABEL_MEASURE_H_

#include "util_types.h"
#include "LabelPosition.h"
#include "Beat.h"

namespace sinsy
{

class LabelMeasure
{
public:
   //! constructor
   LabelMeasure(const Beat& b);

   //! destructor
   virtual ~LabelMeasure();

   //! add position
   void addPosition(const LabelPosition&);

   //! get position
   const LabelPosition& getPosition() const;

   //! set max position
   void setMaxPosition(const LabelPosition&);

   //! get max position
   const LabelPosition& getMaxPosition() const;

   //! get duration
   const INT64 getDuration() const;

   //! set measure index
   void setIndex(size_t i);

   //! get measure index
   size_t getIndex() const;

private:
   //! copy constructor (donot use)
   LabelMeasure(const LabelMeasure&);

   //! assignment operator (donot use)
   LabelMeasure& operator=(const LabelMeasure&);

   //! beat
   Beat beat;

   //! position
   LabelPosition position;

   //! max position
   LabelPosition maxPosition;

   //! measure index
   size_t index;
};

};

#endif // SINSY_LABEL_MEASURE_H_

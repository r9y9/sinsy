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

#include "LabelMeasure.h"
#include "util_log.h"
#include "util_score.h"
#include "ScorePosition.h"

namespace sinsy
{

/*!
 constructor
 */
LabelMeasure::LabelMeasure(const Beat& b) : beat(b), index(0)
{
}

/*!
 destructor
 */
LabelMeasure::~LabelMeasure()
{
}

/*!
 add position
 */
void LabelMeasure::addPosition(const LabelPosition& p)
{
   position += p;
   maxPosition.setCount(position.getCount());
}

/*!
 get position
 */
const LabelPosition& LabelMeasure::getPosition() const
{
   return position;
}

/*!
 set max position
 */
void LabelMeasure::setMaxPosition(const LabelPosition& p)
{
   maxPosition = p;
   maxPosition.setCount(position.getCount());
}

/*!
 get max position
 */
const LabelPosition& LabelMeasure::getMaxPosition() const
{
   return maxPosition;
}

/*!
 get duration
 */
const sinsy::INT64 LabelMeasure::getDuration() const
{
   return BASE_DIVISIONS * 4 * beat.getBeats() / beat.getBeatType();
}

/*!
 set measure index
 */
void LabelMeasure::setIndex(size_t i)
{
   index = i;
}

/*!
 get measure index
 */
size_t LabelMeasure::getIndex() const
{
   return index;
}

};  // namespace sinsy

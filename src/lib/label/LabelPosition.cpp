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

#include "LabelPosition.h"
#include "ScorePosition.h"
#include "util_log.h"
#include "util_score.h"

namespace sinsy
{

/*!
 constructor
 */
LabelPosition::LabelPosition() : count(0), time(0.0), point(0), duration(0)
{
}

/*!
 constructor
 */
LabelPosition::LabelPosition(size_t dur, double tempo) : count(0), time(0.0), point(0), duration(0)
{
   add(dur, tempo);
}

/*!
 copy constructor
 */
LabelPosition::LabelPosition(const LabelPosition& obj) : count(obj.count), time(obj.time), point(obj.point), duration(obj.duration)
{
}

/*!
 destructor
 */
LabelPosition::~LabelPosition()
{
}

/*!
 assignment operator
 */
LabelPosition& LabelPosition::operator=(const LabelPosition & obj)
{
   if (&obj != this) {
      count = obj.count;
      time = obj.time;
      point = obj.point;
      duration = obj.duration;
   }
   return *this;
}

/*!
 operator +=
 */
LabelPosition& LabelPosition::operator+=(const LabelPosition & obj)
{
   count += obj.count;
   time += obj.time;
   point += obj.point;
   duration += obj.duration;
   return *this;
}

/*!
 operator -=
 */
LabelPosition& LabelPosition::operator-=(const LabelPosition & obj)
{
   count -= obj.count;
   time -= obj.time;
   point -= obj.point;
   duration -= obj.duration;
   return *this;
}

/*!
 operator +
 */
LabelPosition LabelPosition::operator+(const LabelPosition& obj) const
{
   LabelPosition ret(*this);
   ret += obj;
   return ret;
}

/*!
 operator -
 */
LabelPosition LabelPosition::operator-(const LabelPosition& obj) const
{
   LabelPosition ret(*this);
   ret -= obj;
   return ret;
}

/*!
 add
 */
void LabelPosition::add(size_t dur, double tempo)
{
   double  t(dur * 60 / (BASE_DIVISIONS * tempo));
   size_t p(dur * 24);

   ++count;
   time += t;
   point += p;
   duration += dur;
}

/*!
 set count
 */
void LabelPosition::setCount(size_t c)
{
   count = c;
}

/*!
 get count
 */
sinsy::INT64 LabelPosition::getCount() const
{
   return count;
}

/*!
 get time
 */
double LabelPosition::getTime() const
{
   return time;
}

/*!
 get point
 */
sinsy::INT64 LabelPosition::getPoint() const
{
   return static_cast<INT64>(point / BASE_DIVISIONS);
}

/*!
 get duration
 */
sinsy::INT64 LabelPosition::getDuration() const
{
   return duration;
}

/*!
 to stream
 */
std::ostream& operator<<(std::ostream& os, const LabelPosition& obj)
{
   os << "count:" << obj.getCount() << ", time:" << obj.getTime() << ", point:" << obj.getPoint() << ", duration:" << obj.getDuration();
   return os;
}

};  // namespace sinsy

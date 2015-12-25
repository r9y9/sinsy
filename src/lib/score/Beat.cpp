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

#include <stdexcept>
#include "Beat.h"

namespace sinsy
{

namespace
{
const size_t DEFAULT_BEATS = 4;
const size_t DEFAULT_BEAT_TYPE = 4;
};

/*!
 constructor
 */
Beat::Beat() : beats(DEFAULT_BEATS), beatType(DEFAULT_BEAT_TYPE)
{}

/*!
 constructor
 */
Beat::Beat(size_t b, size_t t)
{
   setBeats(b);
   setBeatType(t);
}

/*!
 copy constructor
 */
Beat::Beat(const Beat& obj) : beats(obj.beats), beatType(obj.beatType)
{
}

/*!
 destructor
 */
Beat::~Beat()
{
}

/*!
 assignment operator
 */
Beat& Beat::operator=(const Beat & obj)
{
   this->beats = obj.beats;
   this->beatType = obj.beatType;
   return *this;
}

/*!
 get beats
 */
size_t Beat::getBeats() const
{
   return this->beats;
}

/*!
 get beat type
 */
size_t Beat::getBeatType() const
{
   return this->beatType;
}

/*!
 set beats
 */
void Beat::setBeats(size_t b)
{
   if (b <= 0) {
      throw std::range_error("Beat::setBeats() invalid value");
   }
   this->beats = b;
}

/*!
 set beat type
 */
void Beat::setBeatType(size_t b)
{
   if (b <= 0) {
      throw std::range_error("Beat::setBeatType() invalid value");
   }
   this->beatType = b;
}

/*!
 to string
 */
std::ostream& operator<<(std::ostream& os, const Beat& beat)
{
   return os << beat.getBeats() << "/" << beat.getBeatType();
}

};  // namespace sinsy

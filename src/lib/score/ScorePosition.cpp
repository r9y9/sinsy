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

#include "ScorePosition.h"

namespace sinsy
{

/*!
 constructor
 */
ScorePosition::ScorePosition() : measure(0), numerator(0), denominator(0)
{
}

/*!
 constructor
 */
ScorePosition::ScorePosition(size_t m) : measure(m), numerator(0), denominator(0)
{
}

/*!
 constructor
 */
ScorePosition::ScorePosition(size_t m, size_t n, size_t d) : measure(m), numerator(n), denominator(d)
{
}

/*!
 copy constructor
 */
ScorePosition::ScorePosition(const ScorePosition& obj) : measure(obj.measure), numerator(obj.numerator), denominator(obj.denominator)
{
}

/*!
 destructor
 */
ScorePosition::~ScorePosition()
{
}

/*!
 assignment operator
 */
ScorePosition& ScorePosition::operator=(const ScorePosition & obj)
{
   this->measure = obj.measure;
   this->numerator = obj.numerator;
   this->denominator = obj.denominator;
   return *this;
}

/*!
 get measure
 */
size_t ScorePosition::getMeasure() const
{
   return measure;
}

/*!
 get numerator
 */
size_t ScorePosition::getNumerator() const
{
   return numerator;
}

/*!
 get denominator
 */
size_t ScorePosition::getDenominator() const
{
   return denominator;
}

/*!
 to stream
 */
std::ostream& operator<<(std::ostream& os, const ScorePosition& position)
{
   return os << position.getMeasure() << " : " << position.getNumerator() << "/" << position.getDenominator();
}

};  // namespace sinsy

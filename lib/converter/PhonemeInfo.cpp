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

#include "util_log.h"
#include "PhonemeInfo.h"

namespace sinsy
{

const std::string PhonemeInfo::TYPE_SILENT    = "s";
const std::string PhonemeInfo::TYPE_PAUSE     = "p";
const std::string PhonemeInfo::TYPE_VOWEL     = "v";
const std::string PhonemeInfo::TYPE_CONSONANT = "c";
const std::string PhonemeInfo::TYPE_BREAK     = "b";

/*!
 constructor
 */
PhonemeInfo::PhonemeInfo() : scoreFlag(0)
{
}

/*!
 constructor
 */
PhonemeInfo::PhonemeInfo(const std::string& t, const std::string& p, ScoreFlag f) : type(t), phoneme(p), scoreFlag(f)
{
}

/*!
 copy constructor
 */
PhonemeInfo::PhonemeInfo(const PhonemeInfo& obj) : type(obj.type), phoneme(obj.phoneme), scoreFlag(obj.scoreFlag)
{
}

/*!
 destructor
 */
PhonemeInfo::~PhonemeInfo()
{
}

/*!
 assignment operator
 */
PhonemeInfo& PhonemeInfo::operator=(const PhonemeInfo& obj)
{
   this->type = obj.type;
   this->phoneme = obj.phoneme;
   this->scoreFlag = obj.scoreFlag;
   return *this;
}

/*!
 get type
 */
const std::string& PhonemeInfo::getType() const
{
   return this->type;
}

/*!
 get phoneme
 */
const std::string& PhonemeInfo::getPhoneme() const
{
   return this->phoneme;
}

/*!
 set phoneme
 */
void PhonemeInfo::setPhoneme(const std::string& p)
{
   this->phoneme = p;
}

/*!
 return score flag
 */
ScoreFlag PhonemeInfo::getScoreFlag() const
{
   return this->scoreFlag;
}

};  // namespace sinsy

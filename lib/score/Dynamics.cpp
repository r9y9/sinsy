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
#include "Dynamics.h"
#include "util_log.h"
#include "util_string.h"

namespace sinsy
{

namespace
{
const int DYNAMICS_NUM = 11;

const std::string STR_P4 = "p4";
const std::string STR_P3 = "p3";
const std::string STR_P2 = "p2";
const std::string STR_P1 = "p1";
const std::string STR_MP = "mp";
const std::string STR_N = "n";
const std::string STR_MF = "mf";
const std::string STR_F1 = "f1";
const std::string STR_F2 = "f2";
const std::string STR_F3 = "f3";
const std::string STR_F4 = "f4";
const std::string STR_PPPP = "pppp";
const std::string STR_PPP = "ppp";
const std::string STR_PP = "pp";
const std::string STR_P = "p";
const std::string STR_F = "f";
const std::string STR_FF = "ff";
const std::string STR_FFF = "fff";
const std::string STR_FFFF = "ffff";

const std::string DYNAMICSES[] = {
   STR_P4, STR_P3, STR_P2, STR_P1, STR_MP, STR_N, STR_MF, STR_F1, STR_F2, STR_F3, STR_F4
};
const std::string DYNAMICS_TAGS[] = {
   STR_PPPP, STR_PPP, STR_PP, STR_P, STR_MP, STR_N, STR_MF, STR_F, STR_FF, STR_FFF, STR_FFFF
};

class CompDynamics
{
public:
   //! constructor
   CompDynamics(const std::string& str) : target(str) {}

   //! destructor
   virtual ~CompDynamics() {}

   //! ...
   bool operator()(const std::string& str) const {
      return (0 == target.compare(str)) ? true : false;
   }

   //! target
   const std::string& target;
};

};

const Dynamics Dynamics::PPPP(STR_PPPP);
const Dynamics Dynamics::PPP(STR_PPP);
const Dynamics Dynamics::PP(STR_PP);
const Dynamics Dynamics::P(STR_P);
const Dynamics Dynamics::MP(STR_MP);
const Dynamics Dynamics::N(STR_N);
const Dynamics Dynamics::MF(STR_MF);
const Dynamics Dynamics::F(STR_F);
const Dynamics Dynamics::FF(STR_FF);
const Dynamics Dynamics::FFF(STR_FFF);
const Dynamics Dynamics::FFFF(STR_FFFF);

/*!
 constructor
 */
Dynamics::Dynamics() : value((DYNAMICS_NUM - 1) / 2)
{
}

/*!
 constructor
 */
Dynamics::Dynamics(const std::string& str)
{
   set(str);
}

/*!
 copy constructor
 */
Dynamics::Dynamics(const Dynamics& obj) : value(obj.value)
{
}

/*!
 destructor
 */
Dynamics::~Dynamics()
{
}

/*!
 assignment operator
 */
Dynamics& Dynamics::operator=(const Dynamics & obj)
{
   value = obj.value;
   return *this;
}

/*!
 equal
 */
bool Dynamics::operator==(const Dynamics& obj) const
{
   return (obj.value == value);
}

/*!
 not equal
 */
bool Dynamics::operator!=(const Dynamics& obj) const
{
   return !(obj == *this);
}

/*!
 set value
 */
void Dynamics::set(const std::string& _str)
{
   std::string s(_str);
   toLower(s);
   const std::string* itr(std::find_if(DYNAMICSES, DYNAMICSES + DYNAMICS_NUM, CompDynamics(s)));
   if (itr < DYNAMICSES + DYNAMICS_NUM) {
      value = itr - DYNAMICSES;
      return;
   }
   itr = std::find_if(DYNAMICS_TAGS, DYNAMICS_TAGS + DYNAMICS_NUM, CompDynamics(s));
   if (itr < DYNAMICS_TAGS + DYNAMICS_NUM) {
      value = itr - DYNAMICS_TAGS;
      return;
   }
   ERR_MSG("Unexpected dynamics : " << s);
   throw std::runtime_error("Dynamics::set() invalid argument");
}

/*!
 get valie as string
 */
const std::string& Dynamics::getStr() const
{
   return DYNAMICSES[value];
}

/*!
 get tag string
 */
const std::string& Dynamics::getTagStr() const
{
   return DYNAMICS_TAGS[value];
}

/*!
 to string
 */
std::ostream& operator<<(std::ostream& os, const Dynamics& dynamics)
{
   return os << dynamics.getStr();
}

};  // namespace sinsy

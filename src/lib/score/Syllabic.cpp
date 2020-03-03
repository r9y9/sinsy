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
#include <string>
#include <stdexcept>
#include "Syllabic.h"
#include "util_log.h"
#include "util_string.h"

namespace sinsy
{

namespace
{
const int SYLLABIC_NUM = 4;
const std::string STR_SINGLE = "single";
const std::string STR_BEGIN = "begin";
const std::string STR_END = "end";
const std::string STR_MIDDLE = "middle";
const std::string SYLLABICS[] = {
   STR_SINGLE, STR_BEGIN, STR_END, STR_MIDDLE
};

class CompSyllabic
{
public:

   //! constructor
   explicit CompSyllabic(const std::string& str) : target(str) {}

   //! destructor
   virtual ~CompSyllabic() {}

   //! ...
   bool operator()(const std::string& str) const {
      return (0 == target.compare(str)) ? true : false;
   }

   //! target
   const std::string& target;
};

};

const Syllabic Syllabic::SINGLE(STR_SINGLE);
const Syllabic Syllabic::BEGIN(STR_BEGIN);
const Syllabic Syllabic::END(STR_END);
const Syllabic Syllabic::MIDDLE(STR_MIDDLE);

/*!
 constructor
 */
Syllabic::Syllabic() : value(0)
{
}

/*!
 constructor
 */
Syllabic::Syllabic(const std::string& str)
{
   set(str);
}

/*!
 copy constructor
 */
Syllabic::Syllabic(const Syllabic& obj) : value(obj.value)
{
}

/*!
 destructor
 */
Syllabic::~Syllabic()
{
}

/*!
 assignment operator
 */
Syllabic& Syllabic::operator=(const Syllabic & obj)
{
   if (this != &obj) {
      value = obj.value;
   }
   return *this;
}

/*!
 equal
 */
bool Syllabic::operator==(const Syllabic& obj) const
{
   return (obj.value == this->value);
}

/*!
 not equal
 */
bool Syllabic::operator!=(const Syllabic& obj) const
{
   return !(obj == *this);
}

/*!
 set
 */
void Syllabic::set(const std::string& s)
{
   std::string str(s);
   toLower(str);
   const std::string* itr(std::find_if(SYLLABICS, SYLLABICS + SYLLABIC_NUM, CompSyllabic(str)));
   if (itr < SYLLABICS + SYLLABIC_NUM) {
      value = itr - SYLLABICS;
      return;
   }
   ERR_MSG("Unexpected syllabic : " << str);
   throw std::runtime_error("Syllabic::set() invalid argument");
}

/*!
 get syllabic
 */
const std::string& Syllabic::get() const
{
   return SYLLABICS[value];
}

/*!
 to stream
 */
std::ostream& operator<<(std::ostream& os, const Syllabic& syllabic)
{
   return os << syllabic.get();
}

};  // namespace sinsy

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

#include "Mode.h"
#include "util_log.h"
#include "util_string.h"

namespace sinsy
{

namespace
{
const int MODE_NUM = 2;
const std::string STR_MAJOR = "major";
const std::string STR_MINOR = "minor";
const std::string MODES[] = {STR_MAJOR, STR_MINOR};

class CompMode
{
public:
   //! constructor
   explicit CompMode(const std::string& str) : target(str) {}

   //! destructor
   virtual ~CompMode() {}

   //! ...
   bool operator()(const std::string& str) const {
      return (0 == target.compare(str)) ? true : false;
   }

   //! ...
   const std::string& target;
};

};

const Mode Mode::MAJOR(STR_MAJOR);
const Mode Mode::MINOR(STR_MINOR);

/*!
 constructor
 */
Mode::Mode() : value(0)
{
}

/*!
 constructor

 @param str mode
 */
Mode::Mode(const std::string& str)
{
   set(str);
}

/*!
 copy constructor
 */
Mode::Mode(const Mode& obj) : value(obj.value)
{
}

/*!
 destructor
 */
Mode::~Mode()
{
}

/*!
 assignment operator
 */
Mode& Mode::operator=(const Mode & obj)
{
   if (this != &obj) value = obj.value;
   return *this;
}

/*!
 equal operator
 */
bool Mode::operator==(const Mode& obj) const
{
   return (obj.value == this->value);
}

/*!
 not equal operator
 */
bool Mode::operator!=(const Mode& obj) const
{
   return !(obj == *this);
}

/*!
 set mode

 @param str mode
 */
void Mode::set(const std::string& str)
{
   std::string s(str);
   toLower(s);
   const std::string* itr(std::find_if(MODES, MODES + MODE_NUM, CompMode(s)));
   if (itr < MODES + MODE_NUM) {
      value = itr - MODES;
      return;
   }
   ERR_MSG("Unexpected mode : " << s);
   throw std::runtime_error("Mode::set() invalid argument");
}

/*!
 get mode

 @return mode
 */
const std::string& Mode::get() const
{
   return MODES[value];
}

/*!
 to stream
 */
std::ostream& operator<<(std::ostream& os, const Mode& mode)
{
   return os << mode.get();
}

};  // namespace sinsy

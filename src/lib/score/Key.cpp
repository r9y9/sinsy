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
#include "Key.h"
#include "util_log.h"
#include "util_string.h"

namespace sinsy
{

namespace
{

/*!
 adjust fifths
 */
size_t adjustFifths(int f)
{
   while (f < 0) {
      f += 12;
   }
   while (12 <= f) {
      f -= 12;
   }
   return static_cast<size_t>(f);
}

}; // namespace

/*!
 constructor
 */
Key::Key() : fifths(0), origFifths(0)
{
}

/*!
 constructor

 @param m mode
 @param f fifths
 */
Key::Key(const Mode& m, int f) : mode(m), fifths(adjustFifths(f)), origFifths(f)
{
}

/*!
 copy constructor
 */
Key::Key(const Key& obj) : mode(obj.mode), fifths(obj.fifths), origFifths(obj.origFifths)
{
}

/*!
 destructor
 */
Key::~Key()
{
}

/*!
 assignment operator
 */
Key& Key::operator=(const Key & obj)
{
   if (this != &obj) {
      this->mode = obj.mode;
      this->fifths = obj.fifths;
      this->origFifths = obj.origFifths;
   }
   return *this;
}

/*!
 equal operator
 */
bool Key::operator==(const Key& obj) const
{
   return (obj.mode == this->mode) && (obj.fifths == this->fifths) && (obj.origFifths == this->origFifths);
}

/*!
 not equal operator
 */
bool Key::operator!=(const Key& obj) const
{
   return !(obj == *this);
}

/*!
 get mode

 @return mode
 */
const Mode& Key::getMode() const
{
   return this->mode;
}

/*!
 get fifths

 @return fifths
 */
size_t Key::getFifths() const
{
   return this->fifths;
}

/*!
 get original fifths
 */
int Key::getOrigFifths() const
{
   return this->origFifths;
}


/*!
 set mode

 @param m mode
 */
void Key::setMode(const Mode& m)
{
   this->mode = m;
}

/*!
 set fifths
 */
void Key::setFifths(int f)
{
   this->origFifths = f;
   this->fifths = adjustFifths(f);
}

/*!
 to string
 */
std::ostream& operator<<(std::ostream& os, const Key& key)
{
   return os << "mode:" << key.getMode() << ", fifths:" << key.getFifths();
}

};  // namespace sinsy

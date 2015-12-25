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
#include "util_log.h"
#include "Slur.h"

namespace sinsy
{

/*!
 constructor
 */
Slur::Slur()
{
}

/*!
 copy constructor
 */
Slur::Slur(const Slur& obj)
{
   *this = obj;
}

/*!
 destructor
 */
Slur::~Slur()
{
}

/*!
 assignment operator
 */
Slur& Slur::operator=(const Slur & obj)
{
   if (this != &obj) {
      this->startNumberList = obj.startNumberList;
      this->stopNumberList = obj.stopNumberList;
   }
   return *this;
}

/*!
 add start
 */
void Slur::addStart(int number)
{
   NumberList::iterator itr(std::find(startNumberList.begin(), startNumberList.end(), number));
   if (startNumberList.end() == itr) {
      startNumberList.push_back(number);
   }
}

/*!
 @internal
 remove from start number list

 @param number target slur number
 @return whether the number was contained in start number list or not
 */
bool Slur::removeFromStartNumberList(int number)
{
   NumberList::iterator itr(std::find(startNumberList.begin(), startNumberList.end(), number));
   if (startNumberList.end() == itr) { // not contained in start number list
      return false;
   }
   startNumberList.erase(itr);
   return true;
}

/*!
 add stop
 */
void Slur::addStop(int number)
{
   bool exist(removeFromStartNumberList(number));

   NumberList::iterator itr(std::find(stopNumberList.begin(), stopNumberList.end(), number));
   if (!exist && (stopNumberList.end() == itr)) {
      stopNumberList.push_back(number);
   } else if (exist && (stopNumberList.end() != itr)) {
      stopNumberList.erase(itr);
   }
}

/*!
 no slur or not
 */
bool Slur::noSlur() const
{
   if (startNumberList.empty() && stopNumberList.empty()) {
      return true;
   }
   return false;
}

/*!
 merge to another slur
 */
void Slur::mergeTo(Slur& slur) const
{
   {
      const NumberList::const_iterator itrEnd(this->stopNumberList.end());
      for (NumberList::const_iterator itr(this->stopNumberList.begin()); itrEnd != itr; ++itr) {
         slur.addStop(*itr);
      }
   }
   {
      const NumberList::const_iterator itrEnd(this->startNumberList.end());
      for (NumberList::const_iterator itr(this->startNumberList.begin()); itrEnd != itr; ++itr) {
         slur.addStart(*itr);
      }
   }
}

/*!
 clear
 */
void Slur::clear()
{
   startNumberList.clear();
   stopNumberList.clear();
}

/*!
 to stream
 */
std::ostream& operator<<(std::ostream& os, const Slur& slur)
{

   {
      os << "Start: ";
      const Slur::NumberList::const_iterator itrEnd(slur.startNumberList.end());
      for (Slur::NumberList::const_iterator itr(slur.startNumberList.begin()); itrEnd != itr; ++itr) {
         if (slur.startNumberList.begin() != itr) {
            os << ", ";
         }
         os << (*itr);
      }
   }
   os << ", ";
   {
      os << "Stop: ";
      const Slur::NumberList::const_iterator itrEnd(slur.stopNumberList.end());
      for (Slur::NumberList::const_iterator itr(slur.stopNumberList.begin()); itrEnd != itr; ++itr) {
         if (slur.stopNumberList.begin() != itr) {
            os << ", ";
         }
         os << (*itr);
      }
   }
   return os;
}

};  // namespace sinsy

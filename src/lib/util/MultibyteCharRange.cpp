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
#include "util_string.h"
#include "StringTokenizer.h"
#include "MultibyteCharRange.h"

namespace sinsy
{

/*!
 constructor
 */
MultibyteCharRange::MultibyteCharRange()
{
}

/*!
 destructor
 */
MultibyteCharRange::~MultibyteCharRange()
{
}

/*!
 add multibyte char range
 */
bool MultibyteCharRange::addRange(size_t sz, unsigned char b, unsigned char e)
{
   if (sz <= 0) {
      ERR_MSG("Multibyte char size is set to zero");
      return false;
   }
   if (e < b) {
      unsigned char tmp = b;
      b = e;
      e = tmp;
   }

   // check overlapping
   const RangeList::const_iterator itrEnd(rangeList.end());
   RangeList::const_iterator itr(rangeList.begin());
   for (; itrEnd != itr; ++itr) {
      if ((itr->begin <= b && b <= itr->end) || (itr->begin <= e && e <= itr->end)) {
         ERR_MSG("Multibyte char ranges are overlapped");
         return false;
      }
   }

   // add
   rangeList.push_back(Range(sz, b, e));

   return true;
}

/*!
 get char size (if 1 or unknown, return 1)
 */
size_t MultibyteCharRange::getCharSize(unsigned char firstChar) const
{
   const RangeList::const_iterator itrEnd(rangeList.end());
   RangeList::const_iterator itr(rangeList.begin());
   for (; itrEnd != itr; ++itr) {
      if (itr->begin <= firstChar && firstChar <= itr->end) {
         return itr->size;
      }
   }

   return 1;
}

};  // namespace sinsy

/* ----------------------------------------------------------------- */
/*           The HMM-Based Singing Voice Synthesis System "Sinsy"    */
/*           developed by Sinsy Working Group                        */
/*           http://sinsy.sourceforge.net/                           */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2013  Nagoya Institute of Technology          */
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
#include <algorithm>
#include "util_log.h"
#include "ConfGroup.h"
#include "Deleter.h"

using namespace sinsy;

namespace
{
const std::string SIL_STR = "sil";
};

/*!
 constructor
 */
ConfGroup::ConfGroup()
{
}

/*!
 destructor
*/
ConfGroup::~ConfGroup()
{
   clear();
}

/*!
 clear all confs
*/
void ConfGroup::clear()
{
   confs.clear();
}

/*!
 convert lyrics to phonemes
*/
bool ConfGroup::convert(const std::string& enc, ConvertableList::iterator begin, ConvertableList::iterator end) const
{
   const ConfList::const_iterator itrEnd(confs.end());
   for (ConfList::const_iterator itr(confs.begin()); itrEnd != itr; ++itr) {
      ConvertableList::iterator b(begin);
      for (ConvertableList::iterator i(b); ; ++i) {
         if ((end == i) || (*i)->isConverted()) {
            (*itr)->convert(enc, b, i);
            b = i;
            if (end == i) break;
         }
      }
   }
   return true;
}

/*!
 add config
 */
void ConfGroup::add(const IConf* conf)
{
   if (NULL == conf) {
      std::runtime_error("ConfGroup::addConf() NULL pointer");
   }
   confs.push_back(conf);
}

/*!
 get sil string
 */
std::string ConfGroup::getSilStr() const
{
   if (!confs.empty()) {
      return confs.front()->getSilStr();
   }
   return SIL_STR;
}

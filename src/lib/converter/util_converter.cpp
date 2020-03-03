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

#include "util_converter.h"
#include "util_log.h"
#include "util_string.h"
#include "StringTokenizer.h"
#include "MultibyteCharRange.h"
#include <iomanip>

namespace sinsy
{

namespace
{
const size_t SCORE_FLAG_NUM = 8;
const char SCORE_FLAG_CHARS[SCORE_FLAG_NUM] = {'\0', '\0', '\0', '#', '$', '@', '^', '%'}; // '\0': Not defined
const ScoreFlag SCORE_FLAG_DISABLE = 0x10;
const ScoreFlag SCORE_FLAG_FALSETTO = 0x08;
const ScoreFlag SCORE_FLAGS[SCORE_FLAG_NUM] = {0x80, 0x40, 0x20, SCORE_FLAG_DISABLE, SCORE_FLAG_FALSETTO, 0x04, 0x02, 0x01};
/*!
 find char from str
 */
size_t findChar(std::string& str, char c, const MultibyteCharRange* charRange, size_t startIdx = 0)
{
   size_t sz(str.size());
   size_t idx(startIdx);
   while (idx < sz) {
      if (c == str[idx]) {
         return idx;
      }
      if (charRange) {
         idx += charRange->getCharSize(static_cast<unsigned char>(str[idx]));
      } else {
         ++idx;
      }
   }
   return std::string::npos;
}

ScoreFlag matchScoreFlag(char c)
{
   for (size_t i(0); i < SCORE_FLAG_NUM; ++i) {
      if (SCORE_FLAG_CHARS[i] == c) {
         return SCORE_FLAGS[i];
      }
   }
   return 0x00;
}

}; // namespace

/*!
 analyze score flags
 */
ScoreFlag analyzeScoreFlags(std::string& str, const MultibyteCharRange* charRange)
{
   ScoreFlag ret(0x00);

   size_t idx(0);
   while (idx < str.size()) {
      ScoreFlag f(matchScoreFlag(str[idx]));
      if (0x00 == f) { // not found
         if (charRange) {
            idx += charRange->getCharSize(static_cast<unsigned char>(str[idx]));
         } else {
            ++idx;
         }
      } else { // found
         ret |= f;
         str.erase(idx, 1);
      }
   }
   return ret;
}

std::string restoreScoreFlag(ScoreFlag flag)
{
   if (0 == flag) {
      return std::string();
   }
   std::ostringstream oss;
   for (size_t i(0); i < SCORE_FLAG_NUM; ++i) {
      if (SCORE_FLAGS[i] & flag) {
         oss << SCORE_FLAG_CHARS[i];
      }
   }
   return oss.str();
}

bool isEnableFlag(ScoreFlag flag)
{
   return (0x00 == (SCORE_FLAG_DISABLE & flag));
}

std::string getScoreFlagStr(ScoreFlag flag)
{
   std::ostringstream oss;
   oss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(flag);
   return oss.str();
}

ScoreFlag& setDisableFlag(ScoreFlag& flag)
{
   flag |= SCORE_FLAG_DISABLE;
   return flag;
}

ScoreFlag& unsetDisableFlag(ScoreFlag& flag)
{
   flag &= ~SCORE_FLAG_DISABLE;
   return flag;
}

ScoreFlag& setFalsettoFlag(ScoreFlag& flag)
{
   flag |= SCORE_FLAG_FALSETTO;
   return flag;
}


};  // namespace sinsy

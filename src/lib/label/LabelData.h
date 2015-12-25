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

#ifndef SINSY_LABEL_DATA_H_
#define SINSY_LABEL_DATA_H_

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>
#include "util_types.h"

namespace sinsy
{

class LabelData
{
public:
   static const INT64 INVALID_TIME;

   //! constructor
   LabelData();

   //! destructor
   virtual ~LabelData();

   //! set monophone flag
   void setMonophoneFlag(bool b);

   //! set output time flag
   void setOutputTimeFlag(bool b);

   //! set begin time
   void setBeginTime(double d);

   //! set end time
   void setEndTime(double d);

   //! set label data
   template <class T>
   void set(char category, size_t number, const T&);

   //! get label data
   const std::string& get(char category, size_t number) const;

private:
   //! monophone flag
   bool monophoneFlag;

   //! output time flag
   bool outputTimeFlag;

   //! begin time
   INT64 beginTime;

   //! end time
   INT64 endTime;

   typedef std::vector<std::string> List;

   typedef std::map<char, List*> Data;

   //! label data
   Data data;

   friend std::ostream& operator<<(std::ostream&, const LabelData&);
};

//! set data
template <class T>
void LabelData::set(char category, size_t number, const T& value)
{
   std::ostringstream oss;
   oss << value;
   set(category, number, oss.str());
}

//! set data (std::string)
template<>
void LabelData::set<std::string>(char category, size_t number, const std::string& value);

//! set data (bool)
template<>
void LabelData::set<bool>(char category, size_t number, const bool& value);

//! to stream
std::ostream& operator<<(std::ostream&, const LabelData&);
};

#endif // SINSY_LABEL_DATA_H_

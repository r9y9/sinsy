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

#ifndef SINSY_MULTIBYTE_CHAR_RANGE_H_
#define SINSY_MULTIBYTE_CHAR_RANGE_H_

#include <string>
#include <vector>
#include "IConf.h"
#include "Configurations.h"
#include "PhonemeTable.h"
#include "MacronTable.h"

namespace sinsy
{

class MultibyteCharRange
{
public:
   //! constructor
   MultibyteCharRange();

   //! destructor
   virtual ~MultibyteCharRange();

   //! add multibyte char range
   bool addRange(size_t sz, unsigned char b, unsigned char e);

   //! get char size (if 1 or unknown, return 1)
   size_t getCharSize(unsigned char firstChar) const;

private:
   //! copy constructor (donot use)
   MultibyteCharRange(const MultibyteCharRange&);

   //! assignment operator (donot use)
   MultibyteCharRange& operator=(const MultibyteCharRange&);

   struct Range {
      //! default constructor
      Range() : size(0), begin(0), end(0) {}

      //! constructor
      Range(size_t s, unsigned char b, unsigned char e) : size(s), begin(b), end(e) {}

      //! size of multibyte char
      size_t size;

      //! begin code
      unsigned char begin;

      //! end code
      unsigned char end;
   };

   typedef std::vector<Range> RangeList;

   //! list of range
   RangeList rangeList;
};

};

#endif // SINSY_MULTIBYTE_CHAR_RANGE_H_

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

#ifndef SINSY_HTS_ENGINE_H_
#define SINSY_HTS_ENGINE_H_

#include <vector>
#include <string>
#include "util_log.h"
#include "HTS_engine.h"

namespace sinsy
{
class LabelStrings;
class SynthConditionImpl;

class HtsEngine
{
public:
   //! constructor
   HtsEngine();

   //! destructor
   virtual ~HtsEngine();

   //! reset
   void reset();

   //! load voices
   bool load(const std::vector<std::string>&);

   //! synthesize
   bool synthesize(const LabelStrings& label, SynthConditionImpl& condition);

   //! stop synthesizing
   void stop();

   //! reset stop flag
   void resetStopFlag();

   //! set alpha
   bool setAlpha(double);

   //! set tone
   bool setTone(double);

   //! set speed
   bool setSpeed(double);

   //! set volume
   bool setVolume(double);

   //! set interpolation weight
   bool setInterpolationWeight(size_t, double);

   size_t get_sampling_frequency();

private:
   //! copy constructor (donot use)
   explicit HtsEngine(const HtsEngine&);

   //! assignment operator (donot use)
   HtsEngine& operator=(const HtsEngine&);

   //! initialize
   void init();

   //! clear
   void clear();

   //! hts_engine API
   HTS_Engine engine;

   //! default frame period
   size_t fperiod;
};

};

#endif // SINSY_HTS_ENGINE_H_

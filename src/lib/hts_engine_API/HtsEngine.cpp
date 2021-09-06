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

#include <math.h>
#include <string.h>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <limits.h>
#include "HtsEngine.h"
#include "LabelStrings.h"
#include "SynthConditionImpl.h"

namespace sinsy
{

/*!
 constructor
 */
HtsEngine::HtsEngine()
{
   init();
}

/*!
 destructor
 */
HtsEngine::~HtsEngine()
{
   clear();
}

/*!
 initialize
 */
void HtsEngine::init()
{
   HTS_Engine_initialize(&engine);
   fperiod = 0;
}

/*!
 clear
 */
void HtsEngine::clear()
{
   HTS_Engine_clear(&engine);
}

/*!
 reset
 */
void HtsEngine::reset()
{
   clear();
   init();
}

/*!
 load voices
*/
bool HtsEngine::load(const std::vector<std::string>& voices)
{
   size_t i;
   char **fn_voices = NULL;

   // check
   if (voices.size() == 0)
      return false;

   // get HTS voice file names
   fn_voices = (char **) malloc(voices.size() * sizeof(char *));
   if (NULL == fn_voices) {
      throw std::bad_alloc();
   }
   for(i = 0; i < voices.size(); i++) {
      fn_voices[i] = strdup(voices[i].c_str());
   }

   // load HTS voices
   if(HTS_Engine_load(&engine, fn_voices, voices.size()) != TRUE) {
      HTS_Engine_clear(&engine);
      for(i = 0; i < voices.size(); i++) {
         free(fn_voices[i]);
      }
      free(fn_voices);
      return false;
   }

   for(i = 0; i < voices.size(); i++) {
      free(fn_voices[i]);
   }
   free(fn_voices);

   // save default frame period
   fperiod = HTS_Engine_get_fperiod(&engine);

   // set audio buffer size (100ms)
   HTS_Engine_set_audio_buff_size(&engine, (size_t) ((double) HTS_Engine_get_sampling_frequency(&engine) * 0.100));

   // phoneme alignment
   HTS_Engine_set_phoneme_alignment_flag(&engine, TRUE);

   return true;
}

/*!
 synthesize
*/
bool HtsEngine::synthesize(const LabelStrings& label, SynthConditionImpl& condition)
{
   // check
   if (HTS_Engine_get_nvoices(&engine) == 0 || label.size() == 0) {
      return false;
   }

   bool playFlag = condition.playFlag;
   bool saveFlag = !condition.saveFilePath.empty();
   bool storeFlag = (NULL != condition.waveformBuffer);

   // nothing to do
   if (!playFlag && !saveFlag && !storeFlag) {
      return true;
   }

   FILE* fp(NULL);
   if (saveFlag) {
      fp = fopen(condition.saveFilePath.c_str(), "wb");
      if (NULL == fp) {
         return false;
      }
   }

   size_t x = HTS_Engine_get_audio_buff_size(&engine);
   if (playFlag) {
      HTS_Engine_set_audio_buff_size(&engine, x); // reset audio device
   } else {
      HTS_Engine_set_audio_buff_size(&engine, 0);
   }

   int error = 0; // 0: no error 1: unknown error 2: bad alloc
   if(HTS_Engine_synthesize_from_strings(&engine, (char**) label.getData(), label.size()) != TRUE) {
      error = 1;
   }

   if (saveFlag) {
      if(0 == error)
         HTS_Engine_save_riff(&engine, fp);
      fclose(fp);
   }
   if (storeFlag && 0 == error) {
      if(condition.waveformBuffer) {
         size_t numSamples = HTS_Engine_get_nsamples(&engine);
         condition.waveformBuffer->resize(numSamples);
         for (size_t i = 0; i < numSamples; ++i)
            (*condition.waveformBuffer)[i] = HTS_Engine_get_generated_speech(&engine, i);
      }
   }

   HTS_Engine_set_audio_buff_size(&engine, x);

   HTS_Engine_refresh(&engine);

   if (2 == error) {
      throw std::bad_alloc();
   }
   return (0 == error);
}

/*!
 stop
*/
void HtsEngine::stop()
{
   HTS_Engine_set_stop_flag(&engine, true);
}

/*!
 reset stop flag
*/
void HtsEngine::resetStopFlag()
{
   HTS_Engine_set_stop_flag(&engine, false);
}

/*!
 set alpha
*/
bool HtsEngine::setAlpha(double alpha)
{
   if (0 == HTS_Engine_get_nvoices(&engine)) {
      return false;
   }

   HTS_Engine_set_alpha(&engine, alpha);
   return true;
}

/*!
 set tone
*/
bool HtsEngine::setTone(double tone)
{
   if (0 == HTS_Engine_get_nvoices(&engine)) {
      return false;
   }

   HTS_Engine_add_half_tone(&engine, tone);
   return true;
}

/*!
 set speed
*/
bool HtsEngine::setSpeed(double speed)
{
   if (0 == HTS_Engine_get_nvoices(&engine)) {
      return false;
   }
   if(0.0 == speed) {
      return false;
   }

   HTS_Engine_set_fperiod(&engine, static_cast<size_t>(fperiod / speed));
   return true;
}

/*!
 set volume
*/
bool HtsEngine::setVolume(double volume)
{
   if (0 == HTS_Engine_get_nvoices(&engine)) {
      return false;
   }

   HTS_Engine_set_volume(&engine, volume);
   return true;
}

/*!
 set interpolation weight
*/
bool HtsEngine::setInterpolationWeight(size_t index, double weight)
{
   if (0 == HTS_Engine_get_nvoices(&engine)) {
      return false;
   }
   if (HTS_Engine_get_nvoices(&engine) <= index) {
      return false;
   }

   HTS_Engine_set_duration_interpolation_weight(&engine, index, weight);
   for(size_t i = 0; i < HTS_Engine_get_nstream(&engine); i++) {
      HTS_Engine_set_parameter_interpolation_weight(&engine, index, i, weight);
      HTS_Engine_set_gv_interpolation_weight(&engine, index, i, weight);
   }

   return true;
}

size_t HtsEngine::get_sampling_frequency() {
   return HTS_Engine_get_sampling_frequency(&engine);
}


};  // namespace sinsy

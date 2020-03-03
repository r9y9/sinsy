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

#ifndef SINSY_SYNTH_CONDITION_IMPL_H_
#define SINSY_SYNTH_CONDITION_IMPL_H_

#include <string>

namespace sinsy
{

class SynthConditionImpl
{
public:
   //! constructor
   SynthConditionImpl();

   //! destructor
   virtual ~SynthConditionImpl();

   //! set play flag
   void setPlayFlag();

   //! unset play flag
   void unsetPlayFlag();

   //! set file path to save RIFF format file
   void setSaveFilePath(const std::string& filePath);

   //! unset file path to save RIFF format file
   void unsetSaveFilePath();

   //! set waveform buffer
   void setWaveformBuffer(std::vector<double>& waveform);

   //! unset waveform buffer
   void unsetWaveformBuffer();


private:
   //! copy constructor (donot use)
   SynthConditionImpl(const SynthConditionImpl&);

   //! assignment operator (donot use)
   SynthConditionImpl& operator=(const SynthConditionImpl&);

   //! play flag
   bool playFlag;

   // ! file path to save wave data
   std::string saveFilePath;

   //! buffer for wave data
   std::vector<double>* waveformBuffer;

   friend class HtsEngine;
};

};

#endif // SINSY_SYNTH_CONDITION_IMPL_H_

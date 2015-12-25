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

#ifndef SINSY_I_SCORE_WRITABLE_H_
#define SINSY_I_SCORE_WRITABLE_H_

#include <vector>
#include <string>
#include "Beat.h"
#include "Dynamics.h"
#include "Key.h"
#include "Note.h"

namespace sinsy
{

class IScoreWritable
{
public:
   //! destructor
   virtual ~IScoreWritable() {}

   // set encoding
   virtual void setEncoding(const std::string& encoding) = 0;

   //! change tempo
   virtual void changeTempo(double tempo) = 0;

   //! change beat
   virtual void changeBeat(const Beat& beat) = 0;

   //! change dynamics
   virtual void changeDynamics(const Dynamics& dynamics) = 0;

   //! change key
   virtual void changeKey(const Key& key) = 0;

   //! start crescendo
   virtual void startCrescendo() = 0;

   //! start diminuendo
   virtual void startDiminuendo() = 0;

   //! stop crescendo
   virtual void stopCrescendo() = 0;

   //! stop diminuendo
   virtual void stopDiminuendo() = 0;

   //! add note
   virtual void addNote(const Note& note) = 0;
};

};

#endif // SINSY_I_SCORE_WRITABLE_H_

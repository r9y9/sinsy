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

#ifndef SINSY_I_NOTE_LABEL_H_
#define SINSY_I_NOTE_LABEL_H_

namespace sinsy
{

class INoteLabel
{
public:
   //! destructor
   virtual ~INoteLabel() {}

   //! set absolute pitch
   virtual void setAbsolutePitch(const Pitch&) = 0;

   //! set relative pitch
   virtual void setRelativePitch(size_t) = 0;

   //! set key
   virtual void setKey(size_t) = 0;

   //! set beat
   virtual void setBeat(const Beat&) = 0;

   //! set tempo
   virtual void setTempo(double) = 0;

   //! set number of syllables
   virtual void setSyllableNum(size_t) = 0;

   //! set length
   virtual void setLength(const LabelPosition&) = 0;

   //! set position in measure
   virtual void setPositionInMeasure(const LabelPosition&, const LabelPosition&) = 0;

   //! set position in phrase
   virtual void setPositionInPhrase(const LabelPosition&, const LabelPosition&) = 0;

   //! set slur from previous note
   virtual void setSlurFromPrev(bool) = 0;

   //! set slur to next note
   virtual void setSlurToNext(bool) = 0;

   //! set dynamics
   virtual void setDynamics(const Dynamics&) = 0;

   //! set length from previous accent
   virtual void setLengthFromPrevAccent(const LabelPosition&) = 0;

   //! set length to next accent
   virtual void setLengthToNextAccent(const LabelPosition&) = 0;

   //! set length from previous staccato
   virtual void setLengthFromPrevStaccato(const LabelPosition&) = 0;

   //! set length to next staccato
   virtual void setLengthToNextStaccato(const LabelPosition&) = 0;

   //! set position in crescendo
   virtual void setPositionInCrescendo(const LabelPosition&, const LabelPosition&) = 0;

   //! set position in diminuendo
   virtual void setPositionInDiminuendo(const LabelPosition&, const LabelPosition&) = 0;

   //! set pitch difference from previous note
   virtual void setPitchDifferenceFromPrev(int) = 0;

   //! set pitch difference to next note
   virtual void setPitchDifferenceToNext(int) = 0;

   //! set breath between previous and this notes
   virtual void setBreathFromPrev(bool) = 0;

   //! set breath between this and next notes
   virtual void setBreathToNext(bool) = 0;

   //! set previous phrase info
   virtual void setPrevPhraseInfo(size_t, size_t) = 0;

   //! set phrase info
   virtual void setPhraseInfo(size_t, size_t) = 0;

   //! set next phrase info
   virtual void setNextPhraseInfo(size_t, size_t) = 0;
};

};

#endif // SINSY_I_NOTE_LABEL_H_

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

#ifndef SINSY_NOTE_H_
#define SINSY_NOTE_H_

#include <string>
#include "Pitch.h"
#include "Syllabic.h"
#include "Slur.h"

namespace sinsy
{

class Note
{
public:
   //! constructor
   Note();

   //! copy constructor (donot use)
   Note(const Note& obj);

   //! destructor
   virtual ~Note();

   //! set duration
   void setDuration(size_t d);

   //! get duration
   size_t getDuration() const;

   //! set pitch
   void setPitch(const Pitch& p);

   //! get pitch
   const Pitch& getPitch() const;

   //! set rest
   void setRest(bool f);

   //! is rest or not
   bool isRest() const;

   //! set lyric
   void setLyric(const std::string& s);

   //! get lyric
   const std::string& getLyric() const;

   //! set syllabic
   void setSyllabic(const Syllabic& s);

   //! get syllabic
   const Syllabic& getSyllabic() const;

   //! set breath mark
   void setBreathMark(bool f);

   //! has breath mark or not
   bool hasBreathMark() const;

   //! set accent
   void setAccent(bool f);

   //! has accent or not
   bool hasAccent() const;

   //! set staccato
   void setStaccato(bool f);

   //! has staccato or not
   bool hasStaccato() const;

   //! set tie start
   void setTieStart(bool f);

   //! is tie start or not
   bool isTieStart() const;

   //! set tie stop
   void setTieStop(bool f);

   //! is tie stop or not
   bool isTieStop() const;

   //! get slur
   const Slur& getSlur() const;

   //! get slur
   Slur& getSlur();

   //! set slur start
   void setSlurStart(bool f);

   //! is slur start or not
   bool isSlurStart() const;

   //! set slur stop
   void setSlurStop(bool f);

   //! is slur stop or not
   bool isSlurStop() const;

private:
   //! assignment operator (donot use)
   Note& operator=(const Note&);

   //! is rest note or not
   bool restNote;

   //! duration
   size_t duration;

   //! has breath mark or not
   bool breathMark;

   //! has accent or not
   bool accent;

   //! has staccato or not
   bool staccato;

   //! is tie start or not
   bool tieStart;

   //! is tie stop or not
   bool tieStop;

   //! slur
   Slur slur;

   //! is slur start or not
   bool slurStart;

   //! is slur stop or not
   bool slurStop;

   //! pitch
   Pitch pitch;

   //! syllabic
   Syllabic syllabic;

   //! lyric
   std::string lyric;
};

//! to stream
std::ostream& operator<<(std::ostream&, const Note&);
};

#endif // SINSY_NOTE_H_

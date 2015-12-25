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

#include "Note.h"

namespace sinsy
{

/*!
 constructor
 */
Note::Note() : restNote(true), duration(0), breathMark(false), accent(false), staccato(false),
   tieStart(false), tieStop(false), slurStart(false), slurStop(false)
{
}

/*!
 copy constructor
 */
Note::Note(const Note& obj) :
   restNote(obj.restNote), duration(obj.duration), breathMark(obj.breathMark),
   accent(obj.accent), staccato(obj.staccato), tieStart(obj.tieStart), tieStop(obj.tieStop),
   slur(obj.slur), slurStart(obj.slurStart), slurStop(obj.slurStop), pitch(obj.pitch), syllabic(obj.syllabic), lyric(obj.lyric)
{
}

/*!
 destructor
 */
Note::~Note()
{
}

/*!
 set duration
 */
void Note::setDuration(size_t d)
{
   duration = d;
}

/*!
 get duration
 */
size_t Note::getDuration() const
{
   return duration;
}

/*!
 set pitch
 */
void Note::setPitch(const Pitch& p)
{
   pitch = p;
}

/*!
 get pitch
 */
const Pitch& Note::getPitch() const
{
   return pitch;
}

/*!
 set rest
 */
void Note::setRest(bool b)
{
   restNote = b;
}

/*!
 is rest or not
 */
bool Note::isRest() const
{
   return restNote;
}

/*!
 set lyric
 */
void Note::setLyric(const std::string& s)
{
   lyric = s;
}

/*!
 get lyric
 */
const std::string& Note::getLyric() const
{
   return lyric;
}

/*!
 set syllabic
 */
void Note::setSyllabic(const Syllabic& s)
{
   syllabic = s;
}

/*!
 get syllabic
 */
const Syllabic& Note::getSyllabic() const
{
   return syllabic;
}

/*!
 set breath mark
 */
void Note::setBreathMark(bool b)
{
   breathMark = b;
}

/*!
 has breath mark or not
 */
bool Note::hasBreathMark() const
{
   return breathMark;
}

/*!
 set accent
 */
void Note::setAccent(bool b)
{
   accent = b;
}

/*!
 has accent or not
 */
bool Note::hasAccent() const
{
   return accent;
}

/*!
 set staccato
 */
void Note::setStaccato(bool b)
{
   staccato = b;
}

/*!
 has staccato or not
 */
bool Note::hasStaccato() const
{
   return staccato;
}

/*!
 set tie start
 */
void Note::setTieStart(bool b)
{
   tieStart = b;
}

/*!
 is tie start or not
 */
bool Note::isTieStart() const
{
   return tieStart;
}

/*!
 set tie stop
 */
void Note::setTieStop(bool b)
{
   tieStop = b;
}

/*!
 is tie stop or not
 */
bool Note::isTieStop() const
{
   return tieStop;
}

/*!
 get slur
 */
const Slur& Note::getSlur() const
{
   return slur;
}

/*!
 get slur
 */
Slur& Note::getSlur()
{
   return slur;
}

/*!
 set slur start
 */
void Note::setSlurStart(bool b)
{
   slurStart = b;
}

/*!
 is slur start or not
 */
bool Note::isSlurStart() const
{
   return slurStart;
}

/*!
 set slur stop
 */
void Note::setSlurStop(bool b)
{
   slurStop = b;
}

/*!
 is slur stop or not
 */
bool Note::isSlurStop() const
{
   return slurStop;
}

/*!
 to stream
 */
std::ostream& operator<<(std::ostream& os, const Note& note)
{
   if (note.isRest()) {
      os << "[rest(" << note.getDuration() << ") " << note.getLyric();
   } else {
      os << "[pitch(" << note.getDuration() << ") " << note.getPitch() << " " << note.getLyric();
   }

   os << " (tie:";
   if (note.isTieStop()) {
      os << "Stop";
   }
   if (note.isTieStart()) {
      os << "Start";
   }
   os << ")";
   os << " (slur:";
   if (note.isSlurStop()) {
      os << "Stop";
   }
   if (note.isSlurStart()) {
      os << "Start";
   }
   os << ")";
   os << "]";
   return os;
}

};  // namespace sinsy

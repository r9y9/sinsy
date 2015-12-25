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

#include <algorithm>
#include "ScoreDoctor.h"
#include "util_log.h"

namespace sinsy
{

namespace
{
const std::string APOSTROPHE_STR = "&apos;";

/*!
 convert apostrophe string to '
 */
void convertApostrophe(Note& note)
{
   std::string lyric(note.getLyric());
   size_t pos = 0;
   bool changed = false;

   while (std::string::npos != (pos = lyric.find(APOSTROPHE_STR, pos))) {
      lyric[pos] = '\'';
      ++pos;
      lyric.erase(pos, APOSTROPHE_STR.size() - 1);
      changed = true;
   }

   if (changed) {
      note.setLyric(lyric);
   }
}

};

/*!
 constructor
 */
ScoreDoctor::ScoreDoctor() : lastTiedNote(NULL), inTie(false)
{
}

/*!
 destructor
 */
ScoreDoctor::~ScoreDoctor()
{
}

/*!
 add note
 */
void ScoreDoctor::addNote(const Note& n)
{
   Note note(n);

   convertApostrophe(note);

   // slur
   if (!note.getSlur().noSlur()) {
      // clear
      note.setSlurStart(false);
      note.setSlurStop(false);

      bool inSlur(!slur.noSlur());
      note.getSlur().mergeTo(slur);
      note.getSlur().clear();
      if (inSlur) {
         if (slur.noSlur()) {
            note.setSlurStop(true);
         }
      } else {
         if (!slur.noSlur()) {
            note.setSlurStart(true);
         }
      }
   }

   // tie
   if (inTie) {
      if (note.isRest() || (lastTiedNote->getNote().getPitch() != note.getPitch())) {
         if (lastTiedNote->getNote().isTieStart()) {
            lastTiedNote->getNote().setTieStart(false);
         } else {
            lastTiedNote->getNote().setTieStop(true);
         }
         inTie = false;
         lastTiedNote = NULL;
      }
   }
   if (note.isTieStop()) {
      inTie = false;
      lastTiedNote = NULL;
   }
   NoteAdder* na = new NoteAdder(note);
   if (note.isTieStart()) {
      inTie = true;
   }
   if (inTie) {
      lastTiedNote = na;
   }

   tempList.push_back(na);
}

};  // namespace sinsy

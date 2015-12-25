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
#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <iomanip>
#include <stack>
#include "XmlWriter.h"
#include "util_log.h"
#include "Note.h"
#include "Dynamics.h"
#include "Key.h"
#include "Syllabic.h"
#include "ScorePosition.h"
#include "WritableStrStream.h"
#include "xml_tags.h"
#include "util_converter.h"
#include "util_score.h"
#include "Converter.h"

namespace sinsy
{

const XmlWriter::Clef XmlWriter::CLEF_DEFAULT = 0;
const XmlWriter::Clef XmlWriter::CLEF_G = 1;
const XmlWriter::Clef XmlWriter::CLEF_F = 2;
const XmlWriter::Clef XmlWriter::CLEF_C = 3;

namespace
{
const char FLAT = 'b';
const std::string DEFAULT_CLEF = "G";
const std::string DEFAULT_LINE = "0";

/*!
 convert type T to string
 */
template <class T>
std::string toStr(const T& value)
{
   std::ostringstream oss;
   oss << value;
   return oss.str();
}


std::string clefToStr(XmlWriter::Clef clef)
{
   if (XmlWriter::CLEF_DEFAULT == clef) {
      throw std::runtime_error("clefToStr() clef is default");
   }
   if (XmlWriter::CLEF_G == clef) {
      return "G";
   }
   if (XmlWriter::CLEF_F == clef) {
      return "F";
   }
   if (XmlWriter::CLEF_C == clef) {
      return "C";
   }
   throw std::runtime_error("clefToStr() clef is invalid");
}

/*!
 get child data of target tag
 */
XmlData* getChildData(XmlData& parent, const std::string& tag)
{
   XmlData* ret(NULL);
   XmlData::Children::iterator itr(parent.childBegin());
   const XmlData::Children::iterator itrEnd(parent.childEnd());
   for (; itrEnd != itr; ++itr) {
      if ((*itr)->getTag() == tag) {
         ret = *itr;
         break;
      }
   }
   if (NULL == ret) {
      ret = new XmlData(tag);
      parent.addChild(ret);
   }
   return ret;
}

/*!
 add clef tag to head of score
 */
void addClefToHead(XmlData& top, XmlWriter::Clef clefType)
{
   if (XmlWriter::CLEF_DEFAULT == clefType) {
      return;
   }

   XmlData* part(getChildData(top, TAG_PART));
   XmlData* measure(getChildData(*part, TAG_MEASURE));
   XmlData* attributes(getChildData(*measure, TAG_ATTRIBUTES));
   XmlData* clef(getChildData(*attributes, TAG_CLEF));
   XmlData* sign(getChildData(*clef, TAG_SIGN));
   XmlData* line(getChildData(*clef, TAG_LINE));

   sign->setData(clefToStr(clefType));
   line->setData(DEFAULT_LINE);
}

/*!
 change clef

 @return if failed return false
 */
void changeClef(XmlData& data, std::stack<std::string>& tagStack, XmlWriter::Clef clef)
{
   if (!tagStack.empty()) {
      const std::string tag(tagStack.top());

      XmlData::Children::iterator itr(data.childBegin());
      const XmlData::Children::iterator itrEnd(data.childEnd());
      for (; itrEnd != itr; ++itr) {
         if (tag == (*itr)->getTag()) {
            tagStack.pop();
            changeClef(**itr, tagStack, clef);
            tagStack.push(tag);
         }
      }
   } else {
      XmlData::Children::iterator itr(data.childBegin());
      const XmlData::Children::iterator itrEnd(data.childEnd());
      for (; itrEnd != itr; ++itr) {
         if (TAG_SIGN == (*itr)->getTag()) {
            (*itr)->setData(clefToStr(clef));
         } else if (TAG_LINE == (*itr)->getTag()) {
            (*itr)->setData(DEFAULT_LINE);
         }
      }
   }
}

void changeClefTag(XmlData& data, XmlWriter::Clef clef)
{
   if (XmlWriter::CLEF_DEFAULT == clef) {
      return;
   }

   addClefToHead(data, clef);

   std::stack<std::string> tagStack;

   tagStack.push(TAG_CLEF);
   tagStack.push(TAG_ATTRIBUTES);
   tagStack.push(TAG_MEASURE);
   tagStack.push(TAG_PART);

   changeClef(data, tagStack, clef);
}

}; // namespace

/*!
 constructor
 */
XmlWriter::XmlWriter() :
   xmlData(NULL), part(NULL), lastMeasure(NULL), lastMeasureNumber(0), duration(0), clef(CLEF_DEFAULT)
{
   initXmlData();
}

/*!
 destructor
 */
XmlWriter::~XmlWriter()
{
   delete xmlData;
}

/*!
 clear
 */
void XmlWriter::clear()
{
   delete xmlData;
   initXmlData();
}

/*!
 get xml data
 */
const XmlData* XmlWriter::getXmlData() const
{
   return xmlData;
}

/*!
 set encoding
 */
void XmlWriter::setEncoding(const std::string& enc)
{
   encoding = enc;
}

/*!
 change tempo
 */
void XmlWriter::changeTempo(double tempo)
{
   std::ostringstream oss;
   oss << std::fixed << std::setprecision(13) << tempo << std::resetiosflags(std::ios_base::floatfield) << std::setprecision(6);
   const std::string tempoStr(oss.str());

   {
      XmlData* sound = new XmlData(TAG_SOUND);
      sound->addAttribute("tempo", tempoStr);
      addXmlData(sound);
   }

   {
      // for finale
      XmlData* directionTag = new XmlData(TAG_DIRECTION);
      XmlData* directionTypeTag = new XmlData(TAG_DIRECTION_TYPE);

      XmlData* soundTag = new XmlData(TAG_SOUND);
      soundTag->addAttribute("tempo", tempoStr);

      XmlData* metronomeTag = new XmlData(TAG_METRONOME);
      XmlData* beatUnitTag = new XmlData(TAG_BEAT_UNIT, "quarter");
      XmlData* perMinuteTag = new XmlData(TAG_PER_MINUTE, tempoStr);
      metronomeTag->addChild(beatUnitTag);
      metronomeTag->addChild(perMinuteTag);

      directionTypeTag->addChild(metronomeTag);
      directionTag->addChild(directionTypeTag);
      directionTag->addChild(soundTag);
      addXmlData(directionTag);
   }
}

/*!
 change beat
 */
void XmlWriter::changeBeat(const Beat& beat)
{
   XmlData* attributes = new XmlData(TAG_ATTRIBUTES);
   XmlData* time = new XmlData(TAG_TIME);
   attributes->addChild(time);
   {
      std::ostringstream oss;
      oss << beat.getBeats();
      XmlData* beats = new XmlData(TAG_BEATS, oss.str());
      time->addChild(beats);
   }
   {
      std::ostringstream oss;
      oss << beat.getBeatType();
      XmlData* beatType = new XmlData(TAG_BEAT_TYPE, oss.str());
      time->addChild(beatType);
   }
   addXmlData(attributes);

   // adjust position in this measure
   this->duration = duration * beat.getBeats() * lastBeat.getBeatType() / (beat.getBeatType() * lastBeat.getBeats());

   lastBeat = beat;
}

/*!
 change dynamics
 */
void XmlWriter::changeDynamics(const Dynamics& dynamics)
{
   XmlData* direction = new XmlData(TAG_DIRECTION);
   XmlData* directionType = new XmlData(TAG_DIRECTION_TYPE);
   direction->addChild(directionType);
   XmlData* dynamics_ = new XmlData(TAG_DYNAMICS);
   directionType->addChild(dynamics_);
   {
      std::ostringstream oss;
      oss << dynamics.getTagStr();
      XmlData* d = new XmlData(oss.str());
      dynamics_->addChild(d);
   }
   addXmlData(direction);
}

/*!
 change key
 */
void XmlWriter::changeKey(const Key& key)
{
   XmlData* attributes = new XmlData(TAG_ATTRIBUTES);
   XmlData* k = new XmlData(TAG_KEY);
   attributes->addChild(k);
   {
      XmlData* f = new XmlData(TAG_FIFTHS, toStr(key.getOrigFifths()));
      k->addChild(f);

      std::ostringstream oss;
      oss << key.getMode();
      XmlData* m = new XmlData(TAG_MODE, oss.str());
      k->addChild(m);
   }
   addXmlData(attributes);

   lastKey = key;
}

/*!
 start crescendo
 */
void XmlWriter::startCrescendo()
{
   setWedgeTag("crescendo");
}

/*!
 start diminuendo
 */
void XmlWriter::startDiminuendo()
{
   setWedgeTag("diminuendo");
}

/*!
 stop crescendo
 */
void XmlWriter::stopCrescendo()
{
   setWedgeTag("stop");
}

/*!
 stop diminuendo
 */
void XmlWriter::stopDiminuendo()
{
   setWedgeTag("stop");
}

/*!
 add note
 */
void XmlWriter::addNote(const Note& note)
{
   const size_t measureDur = getMeasureDuration(lastBeat);
   size_t dur = note.getDuration();
   bool first = true;

   while (0 < dur) {
      Note n(note);
      size_t d = dur;
      if (measureDur < (this->duration + dur)) { // note duration is over this measure
         d = measureDur - this->duration;
      }

      if (0 < d) { // fail safe
         dur -= d;
         this->duration += d;
         bool last = (0 == dur);

         if (first && last) { // single
            // do nothing
         } else if (first) {
            n.setTieStart(true);
            n.setTieStop(false);
            n.setSlurStart(false);
            if(note.getSyllabic() == Syllabic::SINGLE)
               n.setSyllabic(Syllabic::BEGIN);
            else if(note.getSyllabic() == Syllabic::END)
               n.setSyllabic(Syllabic::MIDDLE);
         } else if (last) {
            n.setLyric("");
            n.setTieStart(false);
            n.setTieStop(true);
            n.setSlurStop(false);
            if(note.getSyllabic() == Syllabic::SINGLE)
               n.setSyllabic(Syllabic::END);
            else if(note.getSyllabic() == Syllabic::BEGIN)
               n.setSyllabic(Syllabic::MIDDLE);
         } else {
            n.setLyric("");
            n.setTieStart(false);
            n.setTieStop(false);
            n.setSlurStart(false);
            n.setSlurStop(false);
            if(note.getSyllabic() != Syllabic::MIDDLE)
               n.setSyllabic(Syllabic::MIDDLE);
         }

         if((lastSyllabic == Syllabic::BEGIN || lastSyllabic == Syllabic::MIDDLE) && n.getSyllabic() == Syllabic::SINGLE) {
            n.setSyllabic(Syllabic::MIDDLE);
         }
         lastSyllabic = n.getSyllabic();

         n.setDuration(d);
         setNoteTag(n); // set tags
         first = false;
      }

      if (measureDur == this->duration) { // measure is full of notes
         fixMeasure();
      }
   }
}

/*!
 @internal

 set note tag
 */
void XmlWriter::setNoteTag(const Note& note)
{
   XmlData* noteTag = new XmlData(TAG_NOTE);
   XmlData* notationsTag = NULL;
   XmlData* articulationsTag = NULL;
   XmlData* technicalTag = NULL;

   if (note.isRest()) {
      noteTag->addChild(new XmlData(TAG_REST)); // rest
   } else {
      // pitch
      XmlData* pitchTag = new XmlData(TAG_PITCH);
      Pitch pitch(note.getPitch());
      std::string stepStr(pitch.getStepStr());
      size_t len = stepStr.length();
      int alter = 0;
      int octave = pitch.getOctave();

      if ((1 < len) && (FLAT == stepStr[len - 1])) { // flat
         ++pitch;
         alter = -1;
         stepStr = pitch.getStepStr();
      }

      if (0 <= lastKey.getOrigFifths()) {
         if (- 1 == alter) {
            pitch -= 2;
            alter = 1;
            stepStr = pitch.getStepStr();
         }
      }

      pitchTag->addChild(new XmlData(TAG_STEP, stepStr)); // step
      if (0 != alter) {
         pitchTag->addChild(new XmlData(TAG_ALTER, toStr(alter))); // alter
      }
      pitchTag->addChild(new XmlData(TAG_OCTAVE, toStr(octave))); // octave
      noteTag->addChild(pitchTag);
   }

   noteTag->addChild(new XmlData(TAG_DURATION, toStr(note.getDuration()))); // duration

   if (!note.isRest()) {
      // tie
      std::string type;
      if (note.isTieStart()) {
         type = "start";
      } else if (note.isTieStop()) {
         type = "stop";
      }
      if (!type.empty()) {
         XmlData* tieTag = new XmlData(TAG_TIE);
         tieTag->addAttribute("type", type);
         noteTag->addChild(tieTag);

         if (NULL == notationsTag) {
            notationsTag = new XmlData(TAG_NOTATIONS);
            noteTag->addChild(notationsTag);
         }
         XmlData* tiedTag = new XmlData(TAG_TIED);
         tiedTag->addAttribute("type", type);
         notationsTag->addChild(tiedTag);
      }
   }

   {
      // slur
      std::string type;
      if (note.isSlurStart()) {
         type = "start";
      } else if (note.isSlurStop()) {
         type = "stop";
      }
      if (!type.empty()) {
         if (NULL == notationsTag) {
            notationsTag = new XmlData(TAG_NOTATIONS);
            noteTag->addChild(notationsTag);
         }
         XmlData* slurTag = new XmlData(TAG_SLUR);
         slurTag->addAttribute("type", type);
         notationsTag->addChild(slurTag);
      }
   }

   // breath
   if (note.hasBreathMark()) {
      if (NULL == notationsTag) {
         notationsTag = new XmlData(TAG_NOTATIONS);
         noteTag->addChild(notationsTag);
      }
      if (NULL == technicalTag) {
         technicalTag = new XmlData(TAG_TECHNICAL);
         notationsTag->addChild(technicalTag);
      }
      XmlData* upBowTag = new XmlData(TAG_UP_BOW);
      technicalTag->addChild(upBowTag);
   }

   // accent
   if (note.hasAccent()) {
      if (NULL == notationsTag) {
         notationsTag = new XmlData(TAG_NOTATIONS);
         noteTag->addChild(notationsTag);
      }
      if (NULL == articulationsTag) {
         articulationsTag = new XmlData(TAG_ARTICULATIONS);
         notationsTag->addChild(articulationsTag);
      }
      XmlData* accentTag = new XmlData(TAG_ACCENT);
      articulationsTag->addChild(accentTag);
   }

   // staccato
   if (note.hasStaccato()) {
      if (NULL == notationsTag) {
         notationsTag = new XmlData(TAG_NOTATIONS);
         noteTag->addChild(notationsTag);
      }
      if (NULL == articulationsTag) {
         articulationsTag = new XmlData(TAG_ARTICULATIONS);
         notationsTag->addChild(articulationsTag);
      }
      XmlData* staccatoTag = new XmlData(TAG_STACCATO);
      articulationsTag->addChild(staccatoTag);
   }

   if (!note.isRest()) {
      // lyric
      const std::string& lyric(note.getLyric());
      XmlData* lyricTag = new XmlData(TAG_LYRIC);
      lyricTag->addChild(new XmlData(TAG_SYLLABIC, toStr(note.getSyllabic()))); // syllabic
      lyricTag->addChild(new XmlData(TAG_TEXT, lyric)); // text
      noteTag->addChild(lyricTag);
   }

   addXmlData(noteTag);
}

/*!
 @internal

 set wedge tag

 @param type type of wedge ("crescendo" or "diminuendo" or "stop")
 */
void XmlWriter::setWedgeTag(const std::string& type)
{
   XmlData* direction = new XmlData(TAG_DIRECTION);
   XmlData* directionType = new XmlData(TAG_DIRECTION_TYPE);
   direction->addChild(directionType);
   XmlData* wedge = new XmlData(TAG_WEDGE);
   directionType->addChild(wedge);
   wedge->addAttribute("type", type);
   addXmlData(direction);
}

/*!
 @internal

 set tags of head measure
 */
void XmlWriter::setHeadMeasureTag()
{
   XmlData* attributes = new XmlData(TAG_ATTRIBUTES);

   // divisions
   attributes->addChild(new XmlData(TAG_DIVISIONS, toStr(BASE_DIVISIONS)));

   addXmlData(attributes);
}

/*!
 @internal

 initialize xml data
 */
void XmlWriter::initXmlData()
{
   XmlData* scorePart = new XmlData("score-part");
   scorePart->addAttribute("id", "P1");
   scorePart->addChild(new XmlData("part-name", "MusicXML Part"));

   XmlData* partList = new XmlData("part-list");
   partList->addChild(scorePart);

   part = new XmlData(TAG_PART);
   part->addAttribute("id", "P1");

   xmlData = new XmlData(TAG_SCORE_PARTWISE);
   xmlData->addAttribute("version", "2.0");
   xmlData->addChild(partList);
   xmlData->addChild(part);

   lastBeat = Beat();
   lastMeasure = NULL;
   lastMeasureNumber = 0;
   lastSyllabic = Syllabic::SINGLE;
   duration = 0;

   // donot initialize clef
}

/*!
 @internal

 fix measure
 */
void XmlWriter::fixMeasure()
{
   this->lastMeasure = NULL;
   this->duration = 0;
}

/*!
 @internal

 get last measure

 @return last measure xml data
 */
XmlData* XmlWriter::getLastMeasure()
{
   if (NULL == lastMeasure) {
      lastMeasure = new XmlData(TAG_MEASURE);
      if (0 == lastMeasureNumber) { // set divisions tag to the first measure
         setHeadMeasureTag();
      }
      ++lastMeasureNumber;
      lastMeasure->addAttribute("number", toStr(lastMeasureNumber));
      part->addChild(lastMeasure);
   }
   return lastMeasure;
}

/*!
 @internal

 add xml data
 */
void XmlWriter::addXmlData(XmlData* data)
{
   XmlData* measure = getLastMeasure();
   measure->addChild(data);
}

/*!
 set clef
 */
void XmlWriter::setClef(Clef c)
{
   if ((CLEF_DEFAULT != c) && (CLEF_G != c) && (CLEF_F != c) && (CLEF_C != c)) {
      throw std::runtime_error("Invalid clef");
   }
   clef = c;
}

/*!
 write xml to stream

 @param stream output stream
 @return if success, true
 */
bool XmlWriter::writeXml(WritableStrStream& stream) const
{
   if (NULL == xmlData) {
      ERR_MSG("Cannot write Xml file (data is not set)");
      return false;
   }

   // change clef tag
   if (CLEF_DEFAULT != clef) {
      changeClefTag(*xmlData, clef);
   }

   stream << "<\?xml version=\"1.0\" encoding=\"" << encoding << "\"\?>\n";
   stream << "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 2.0 Partwise//EN\"\n";
   stream << "                                \"http://www.musicxml.org/dtds/partwise.dtd\">\n";
   stream << *xmlData;

   return true;
}


};  // namespace sinsy

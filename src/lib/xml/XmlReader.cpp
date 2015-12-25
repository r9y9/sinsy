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
#include "util_log.h"
#include "util_score.h"
#include "XmlReader.h"
#include "Note.h"
#include "Dynamics.h"
#include "Key.h"
#include "Syllabic.h"
#include "ScorePosition.h"
#include "xml_tags.h"
#include "util_score.h"

namespace sinsy
{

namespace
{
typedef size_t WedgeType;
WedgeType WEDGE_NONE = 0;
WedgeType WEDGE_CRESCENDO = 1;
WedgeType WEDGE_DECRESCENDO = 2;

/*!
 NoteSetter
 */
class NoteSetter
{
public:
   typedef void (*Func)(Note& note, const XmlData*);

   //! constructor
   NoteSetter(Note& n, Func f) : note(n), func(f) {}

   //! copy constructor
   NoteSetter(const NoteSetter& obj) : note(obj.note), func(obj.func) {}

   //! destructor
   virtual ~NoteSetter() {}

   //! ...
   void operator()(const XmlData* data) {
      (*func)(note, data);
   }

private:
   //! assignment operator (donot use)
   NoteSetter& operator=(const NoteSetter&);

   //! note
   Note& note;

   //! function
   Func func;
};

/*!
 convert string to type T
 */
template <class T>
T to(const std::string& str)
{
   std::istringstream iss(str);
   T ret(0);
   iss >> ret;
   return ret;
}

/*!
 set tie
 */
void setTie(Note& note, const XmlData* data)
{
   std::string type(data->getAttribute("type"));
   if (type.empty()) {
      return;
   }
   if (0 == type.compare("start")) {
      note.setTieStart(true);
   } else if (0 == type.compare("stop")) {
      note.setTieStop(true);
   } else {
      throw std::runtime_error("<tie> tag has unexpected attribute(type)");
   }
}

/*!
 set slur
 */
void setSlur(Note& note, const XmlData* data)
{
   std::string type(data->getAttribute("type"));
   int number(to<int>(data->getAttribute("number")));
   if (type.empty()) {
      return;
   }
   if (0 == type.compare("start")) {
      note.getSlur().addStart(number);
      note.setSlurStart(true);
   } else if (0 == type.compare("stop")) {
      note.getSlur().addStop(number);
      note.setSlurStop(true);
   } else if (0 == type.compare("continue")) {
      // do nothing
   } else {
      throw std::runtime_error("<slur> tag has unexpected attribute(type)");
   }
}

/*!
 set pitch
 */
void setPitch(Note& note, const XmlData* data)
{
   XmlData::Children::const_iterator itr(data->childBegin());
   XmlData::Children::const_iterator itrEnd(data->childEnd());
   std::string step("C");
   Pitch::Octave octave(Pitch::DEFAULT_OCTAVE);
   int alter = 0;
   for (; itr != itrEnd; ++itr) {
      const XmlData* child(*itr);
      std::string tag(child->getTag());
      if (0 == tag.compare(TAG_STEP)) {
         step = child->getData();
      } else if (0 == tag.compare(TAG_ALTER)) {
         std::string value(child->getData());
         if (!value.empty()) {
            alter = to<int>(value);
         }
      } else if (0 == tag.compare(TAG_OCTAVE)) {
         std::string value(child->getData());
         if (!value.empty()) {
            octave = to<Pitch::Octave>(value);
         }
      }
   }
   Pitch p(step, octave, alter);
   note.setPitch(p);
}

/*!
 convert children of lyric tag
 */
void convertLyricChildren(Note& note, const XmlData* data)
{
   std::string tag(data->getTag());
   if (0 == tag.compare(TAG_SYLLABIC)) {
      Syllabic s(data->getData());
      note.setSyllabic(s);
   } else if (0 == tag.compare(TAG_TEXT)) {
      note.setLyric(data->getData());
   }
}

/*!
 convert children of articulations tag
 */
void convertArticulationsChildren(Note& note, const XmlData* data)
{
   std::string tag(data->getTag());
   if (0 == tag.compare(TAG_BREATH_MARK)) {
      note.setBreathMark(true);
   } else if (0 == tag.compare(TAG_ACCENT)) {
      note.setAccent(true);
   } else if (0 == tag.compare(TAG_STACCATO)) {
      note.setStaccato(true);
   }
}

/*!
 convert children of technical tag
 */
void convertTechnicalChildren(Note& note, const XmlData* data)
{
   std::string tag(data->getTag());
   if (0 == tag.compare(TAG_UP_BOW)) {
      note.setBreathMark(true);
   }
}

/*!
 convert children of notations tag
 */
void convertNotationsChildren(Note& note, const XmlData* data)
{
   std::string tag(data->getTag());
   if (0 == tag.compare(TAG_TIE)) {
      setTie(note, data);
   } else if (0 == tag.compare(TAG_TIED)) {
      setTie(note, data);
   } else if (0 == tag.compare(TAG_SLUR)) {
      setSlur(note, data);
   } else if (0 == tag.compare(TAG_ARTICULATIONS)) {
      std::for_each(data->childBegin(), data->childEnd(), NoteSetter(note, &convertArticulationsChildren));
   } else if (0 == tag.compare(TAG_TECHNICAL)) {
      std::for_each(data->childBegin(), data->childEnd(), NoteSetter(note, &convertTechnicalChildren));
   }
}

/*!
 convert children of note tag
 */
void convertNoteChildren(Note& note, const XmlData* data)
{
   std::string tag(data->getTag());
   if (0 == tag.compare(TAG_DURATION)) {
      note.setDuration(to<size_t>(data->getData()));
   } else if (0 == tag.compare(TAG_PITCH)) {
      note.setRest(false);
      setPitch(note, data);
   } else if (0 == tag.compare(TAG_REST)) {
      note.setRest(true);
   } else if (0 == tag.compare(TAG_LYRIC)) {
      std::for_each(data->childBegin(), data->childEnd(), NoteSetter(note, &convertLyricChildren));
   } else if (0 == tag.compare(TAG_TIE)) {
      setTie(note, data);
   } else if (0 == tag.compare(TAG_SLUR)) {
      setSlur(note, data);
   } else if (0 == tag.compare(TAG_NOTATIONS)) {
      std::for_each(data->childBegin(), data->childEnd(), NoteSetter(note, &convertNotationsChildren));
   }
}

class MeasureScore : public TempScore
{
public:
   //! constructor
   MeasureScore() {}

   //! destructor
   virtual ~MeasureScore() {
      clear();
   }

   //! clear
   void clear() {
      TempScore::clear();
      noteAdders.clear();
   }

   //! add note
   void addNote(const Note& note) {
      XmlNoteAdder* n(new XmlNoteAdder(note));
      tempList.push_back(n);
      noteAdders.push_back(n);
   }

   //! adjust duration
   void adjustDuration(size_t duration) {
      size_t totalDur(0);
      NoteAdders::iterator itrEnd(noteAdders.end());
      for (NoteAdders::iterator itr(noteAdders.begin()); itrEnd != itr; ++itr) {
         // measure cannot conclude this note, because duration of notes in this measure is too long
         if (duration == totalDur) {
            WARN_MSG("Number of notes in a measure is too large");
            (*itr)->setDuration(0);
         } else {
            size_t dur((*itr)->getDuration());
            totalDur += dur;
            // note duration is too long
            if (duration < totalDur) {
               WARN_MSG("Duration of notes in a measure is too long");
               (*itr)->setDuration(dur + duration - totalDur);
               totalDur = duration;
            }
         }
      }

      // duration of notes in this measure is too short
      if (totalDur < duration) {
         WARN_MSG("Duration of notes in a measure is too short");
         if (noteAdders.empty()) {
            Note note;
            note.setRest(true);
            this->addNote(note);
         }
         XmlNoteAdder* n(noteAdders.back());
         size_t dur(n->getDuration());
         n->setDuration(dur + duration - totalDur);
      }
   }

private:
   //! copy constructor (donot use)
   MeasureScore(const MeasureScore&);

   //! assignment operator (donot use)
   MeasureScore& operator=(const MeasureScore&);

   class XmlNoteAdder : public ITempData
   {
   public:
      //! constructor
      XmlNoteAdder(const Note& n) : note(n) {}

      //! destructor
      virtual ~XmlNoteAdder() {}

      //! write to score
      virtual void write(IScoreWritable& sm) const {
         if (0 < note.getDuration()) {
            sm.addNote(note);
         }
      }

      //! get duration
      size_t getDuration() const {
         return note.getDuration();
      }

      //! set duration
      void setDuration(size_t d) {
         note.setDuration(d);
      }

   private:
      //! note
      Note note;
   };

   typedef std::vector<XmlNoteAdder*> NoteAdders;

   //! list of note adders
   NoteAdders noteAdders;
};

class XmlContainer
{
public:
   //! constructor
   XmlContainer(IScoreWritable& writable) : scoreWritable(writable), divisions(1), wedge(WEDGE_NONE), tempo(0.0) {}

   //! destructor
   virtual ~XmlContainer() {}

   //! write from xml data
   void write(const XmlData* xmlData) {
      // empty
      if (NULL == xmlData) {
         return;
      }

      if (0 != xmlData->getTag().compare(TAG_SCORE_PARTWISE)) {
         ERR_MSG("First tag of XML file is not equal to <score_partwise>");
         throw std::runtime_error("First tag of XML file is not equal to <score_partwise>");
      }
      XmlContainer container(scoreWritable);
      std::for_each(xmlData->childBegin(), xmlData->childEnd(), Adapter(container, &XmlContainer::convertScorePartwiseChildren));
   }

private:
   //! copy constructor (donot use)
   XmlContainer(const XmlContainer&);

   //! assignment operator (donot use)
   XmlContainer& operator=(const XmlContainer&);

   //! set sound
   void setSound(const XmlData* data) {
      std::string tempoStr(data->getAttribute("tempo"));
      if (tempoStr.empty()) {
         return;
      }
      double t(to<double>(tempoStr));
      if (tempo != t) {
         measureScore.changeTempo(t);
         tempo = t;
      }
   }

   //! set wedge
   void setWedge(const XmlData* data) {
      std::string type(data->getAttribute("type"));
      if (type.empty()) {
         return;
      }
      if (0 == type.compare("crescendo")) {
         measureScore.startCrescendo();
         wedge = WEDGE_CRESCENDO;
      } else if (0 == type.compare("diminuendo")) {
         measureScore.startDiminuendo();
         wedge = WEDGE_DECRESCENDO;
      } else if (0 == type.compare("stop")) {
         if (WEDGE_CRESCENDO == wedge) {
            measureScore.stopCrescendo();
         } else if (WEDGE_DECRESCENDO == wedge) {
            measureScore.stopDiminuendo();
         } else {
            WARN_MSG("wedge stop tag is appeared, but crescendo and decrescendo are not set");
         }
         wedge = WEDGE_NONE;
      }
   }

   //! set key
   void setKey(const XmlData* data) {
      XmlData::Children::const_iterator itr(data->childBegin());
      XmlData::Children::const_iterator itrEnd(data->childEnd());
      bool changed(false);
      for (; itr != itrEnd; ++itr) {
         const XmlData* child(*itr);
         std::string tag(child->getTag());
         std::string value(child->getData());
         if (0 == tag.compare(TAG_FIFTHS)) {
            if (value.empty()) {
               WARN_MSG("<fifths> tag in XML file has no value");
            }
            key.setFifths(to<int>(value));
            changed = true;
         } else if (0 == tag.compare(TAG_MODE)) {
            if (value.empty()) {
               WARN_MSG("<mode> tag in XML file has no value");
            }
            key.setMode(Mode(value));
            changed = true;
         }
      }
      if (changed) {
         measureScore.changeKey(key);
      }
   }

   //! set time
   void setTime(const XmlData* data) {
      XmlData::Children::const_iterator itr(data->childBegin());
      XmlData::Children::const_iterator itrEnd(data->childEnd());
      bool changed(false);
      for (; itr != itrEnd; ++itr) {
         const XmlData* child(*itr);
         std::string tag(child->getTag());
         std::string value(child->getData());
         if (0 == tag.compare(TAG_BEATS)) {
            if (value.empty()) {
               WARN_MSG("<beats> tag in XML file has no value");
            }
            beat.setBeats(to<size_t>(value));
            changed = true;
         } else if (0 == tag.compare(TAG_BEAT_TYPE)) {
            if (value.empty()) {
               WARN_MSG("<beat_type> tag in XML file has no value");
            }
            beat.setBeatType(to<size_t>(value));
            changed = true;
         }
      }
      if (changed) {
         measureScore.changeBeat(beat);
      }
   }

   //! set note
   void setNote(const XmlData* data) {
      Note note;
      std::for_each(data->childBegin(), data->childEnd(), NoteSetter(note, &convertNoteChildren));

      note.setDuration(note.getDuration() * BASE_DIVISIONS / divisions); // divisions : 960

      measureScore.addNote(note);
   }

   //! convert attributes children
   void convertAttributesChildren(const XmlData* data) {
      std::string tag(data->getTag());
      if (0 == tag.compare(TAG_DIVISIONS)) {
         divisions = to<size_t>(data->getData());
      } else if (0 == tag.compare(TAG_KEY)) {
         setKey(data);
      } else if (0 == tag.compare(TAG_TIME)) {
         setTime(data);
      } else if (0 == tag.compare(TAG_SOUND)) {
         setSound(data);
      }
   }

   //! convert dynamics children
   void convertDynamicsChildren(const XmlData* data) {
      std::string tag(data->getTag());
      Dynamics d(tag);
      measureScore.changeDynamics(d);
   }

   //! convert direction-type children
   void convertDirectionTypeChildren(const XmlData* data) {
      std::string tag(data->getTag());
      if (0 == tag.compare(TAG_WEDGE)) {
         setWedge(data);
      } else if (0 == tag.compare(TAG_DYNAMICS)) {
         std::for_each(data->childBegin(), data->childEnd(), Adapter(*this, &XmlContainer::convertDynamicsChildren));
      }
   }

   //! convert direction children
   void convertDirectionChildren(const XmlData* data) {
      std::string tag(data->getTag());
      if (0 == tag.compare(TAG_DIRECTION_TYPE)) {
         std::for_each(data->childBegin(), data->childEnd(), Adapter(*this, &XmlContainer::convertDirectionTypeChildren));
      } else if (0 == tag.compare(TAG_SOUND)) {
         setSound(data);
      }
   }

   //! convert measure children
   void convertMeasureChildren(const XmlData* data) {
      std::string tag(data->getTag());
      if (0 == tag.compare(TAG_NOTE)) {
         setNote(data);
      } else if (0 == tag.compare(TAG_ATTRIBUTES)) {
         std::for_each(data->childBegin(), data->childEnd(), Adapter(*this, &XmlContainer::convertAttributesChildren));
      } else if (0 == tag.compare(TAG_SOUND)) {
         setSound(data);
      } else if (0 == tag.compare(TAG_DIRECTION)) {
         std::for_each(data->childBegin(), data->childEnd(), Adapter(*this, &XmlContainer::convertDirectionChildren));
      }
   }

   //! convert pert children
   void convertPartChildren(const XmlData* data) {
      if (0 == data->getTag().compare(TAG_MEASURE)) {
         std::for_each(data->childBegin(), data->childEnd(), Adapter(*this, &XmlContainer::convertMeasureChildren));
         measureScore.adjustDuration(getMeasureDuration(beat));
         scoreWritable << measureScore;
         measureScore.clear();
      }
   }

   //! convert score-partwise children
   void convertScorePartwiseChildren(const XmlData* data) {
      if (0 == data->getTag().compare(TAG_PART)) {
         std::for_each(data->childBegin(), data->childEnd(), Adapter(*this, &XmlContainer::convertPartChildren));
      }
   }

   typedef ForEachAdapter<XmlContainer, const XmlData*> Adapter;

   // writable score
   IScoreWritable& scoreWritable;

   //! key
   Key key;

   //! beat
   Beat beat;

   //! divisions
   size_t divisions;

   //! measure score
   MeasureScore measureScore;

   //! wedge
   WedgeType wedge;

   //! tempo
   double tempo;
};

}; // namespace

/*!
 constructor
 */
XmlReader::XmlReader() :
   xmlData(NULL), part(NULL)
{
   initXmlData();
}

/*!
 destructor
 */
XmlReader::~XmlReader()
{
   delete xmlData;
}

/*!
 clear
 */
void XmlReader::clear()
{
   delete xmlData;
   initXmlData();
}

/*!
 write score
 */
void XmlReader::write(IScoreWritable& writable) const
{
   writable.setEncoding(encoding);
   XmlContainer container(writable);
   container.write(xmlData);
}

/*!
 @internal

 initialize xml data
 */
void XmlReader::initXmlData()
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
}

/*!
 read xml from stream

 @param stream input stream
 @return if success, true
 */
bool XmlReader::readXml(IReadableStream& stream)
{
   try {
      delete xmlData;
      xmlData = NULL;

      xmlData = parser.read(stream, encoding);
   } catch (const StreamException& ex) {
      ERR_MSG("XML file reading error : " << ex.what());
      return false;
   }
   return true;
}

/*!
 get xml data
 */
const XmlData* XmlReader::getXmlData() const
{
   return this->xmlData;
}

};  // namespace sinsy

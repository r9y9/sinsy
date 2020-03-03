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
#include <stdexcept>
#include <sstream>
#include "util_log.h"
#include "NoteLabeler.h"
#include "SyllableLabeler.h"
#include "PhonemeLabeler.h"
#include "util_converter.h"
#include "Deleter.h"
#include "NoteGroup.h"
#include "LabelMeasure.h"
#include "ScorePosition.h"

namespace sinsy
{

namespace
{

/*!
 calculate difference of pitches
 */
int calcPitchDifference(const Pitch& p1, const Pitch& p2)
{
   int tmp1(p1.getOctave() * 12 + p1.getStep());
   int tmp2(p2.getOctave() * 12 + p2.getStep());
   return (tmp1 - tmp2);
}

};

/*!
 constructor

 @param b beat
 @param d dynamics
 @param k key
 */
NoteLabeler::NoteLabeler(const Beat& b, const Dynamics& d, const Key& k) :
   prevNote(NULL), nextNote(NULL), beat(b), dynamics(d), key(k),
   inSlurFromPrev(false), inSlurToNext(false), pitchDifferenceFromPrev(0), pitchDifferenceToNext(0),
   hasPrevPitch(false), hasNextPitch(false), measure(NULL), phrase(NULL),
   crescendo(NULL), diminuendo(NULL), prevPhrase(NULL), nextPhrase(NULL)
{
}

/*!
 destructor
 */
NoteLabeler::~NoteLabeler()
{
   std::for_each(children.begin(), children.end(), Deleter<SyllableLabeler>());
   std::for_each(dataList.begin(), dataList.end(), Deleter<NoteData>());
}

/*!
 is rest or not
 */
bool NoteLabeler::isRest() const
{
   if (dataList.empty()) {
      ERR_MSG("Empty note : " << this);
      throw std::runtime_error("NoteLabeler::isRest() dataList is NULL");
   }
   return dataList.front()->getNote().isRest();
}

/*!
 already converted or not
 */
bool NoteLabeler::isConverted() const
{
   return !children.empty();
}

/*!
 @internal

 connect lyrics

 @return connected lyric
 */
std::string NoteLabeler::connectLyrics()
{
   if (dataList.empty()) {
      return std::string();
   }
   std::ostringstream oss;
   DataList::const_iterator itr(dataList.begin());
   DataList::const_iterator itrEnd(dataList.end());
   for (; itr != itrEnd; ++itr) {
      oss << (*itr)->getNote().getLyric();
   }
   return oss.str();
}

/*!
 get lyric
 */
std::string NoteLabeler::getLyric() const
{
   if (dataList.empty()) {
      throw std::runtime_error("NoteLabeler::getLyric() dataList is empty");
   }
   return this->lyric;
}

/*!
 get syllabic
 */
Syllabic NoteLabeler::getSyllabic() const
{
   if (dataList.empty()) {
      throw std::runtime_error("NoteLabeler::getSyllabic() dataList is empty");
   }

   Syllabic ret(dataList.front()->getNote().getSyllabic());
   const DataList::const_iterator itrEnd(dataList.end());
   for (DataList::const_iterator itr(dataList.begin() + 1); itrEnd != itr; ++itr) {
      Syllabic syllabic((*itr)->getNote().getSyllabic());
      if (Syllabic::BEGIN == syllabic) {
         if (Syllabic::END == ret) {
            ret = Syllabic::MIDDLE;
         } else {
            ret = Syllabic::BEGIN;
         }
      } else if (Syllabic::END == syllabic) {
         if (Syllabic::BEGIN == ret) {
            ret = Syllabic::SINGLE;
         } else {
            ret = Syllabic::END;
         }
      } else if (Syllabic::MIDDLE == syllabic) {
         if (Syllabic::SINGLE == ret) {
            ret = Syllabic::MIDDLE;
         }
      }
   }

   return ret;
}

/*!
 get duration
 */
size_t NoteLabeler::getDuration() const
{
   size_t ret(0);
   DataList::const_iterator itr(dataList.begin());
   const DataList::const_iterator itrEnd(dataList.end());
   for (; itr != itrEnd; ++itr) {
      ret += (*itr)->getNote().getDuration();
   }
   return ret;
}

/*!
 add information

 @param phonemes     phoneme list
 @param language     language
 @param info         language dependent information
 */
void NoteLabeler::addInfo(const std::vector<PhonemeInfo>& phonemes, const std::string& language, const std::string& info)
{
   SyllableLabeler* sLabeler(new SyllableLabeler(language));
   sLabeler->setInfo(info);
   std::vector<PhonemeInfo>::const_iterator itr(phonemes.begin());
   const std::vector<PhonemeInfo>::const_iterator itrEnd(phonemes.end());
   for (; itrEnd != itr; ++itr) {
      PhonemeLabeler* pLabeler(new PhonemeLabeler(*itr));
      sLabeler->addChild(pLabeler);
   }
   children.push_back(sLabeler);
   if (phrase) {
      phrase->addSyllableNum(1);
   }
   if (language.empty() && !this->isRest()) {
      if (NULL == measure) {
         WARN_MSG("Lyric in unknown language : " << getLyric());
      } else {
         WARN_MSG("Lyric in unknown language : " << getLyric() << " in measure " << (measure->getIndex() + 1) << "");
      }
   }
}

/*!
 set positions
 */
void NoteLabeler::setPositions()
{
   const List::iterator itrEnd(children.end());
   size_t sz(children.size());
   size_t pos(0);
   for (List::iterator itr(children.begin()); itrEnd != itr; ++itr) {
      (*itr)->setIdxInNote(pos);
      ++pos;
      (*itr)->setNumInNote(sz);

      (*itr)->setPhonemePositions();
   }
}

/*!
 set label

 @param label label
 */
void NoteLabeler::setLabel(INoteLabel& label) const
{
   if (!isRest()) {
      const Note& note(dataList.front()->getNote());
      label.setAbsolutePitch(note.getPitch());

      Pitch::Step step(note.getPitch().getStep());
      if (Mode::MINOR == key.getMode()) {
         step += 3;
      }

      switch (key.getFifths()) {
      case 0:
         step -= Pitch::C;
         break;
      case 1:
         step -= Pitch::G;
         break;
      case 2:
         step -= Pitch::D;
         break;
      case 3:
         step -= Pitch::A;
         break;
      case 4:
         step -= Pitch::E;
         break;
      case 5:
         step -= Pitch::B;
         break;
      case 6:
         step -= Pitch::Gb;
         break;
      case 7:
         step -= Pitch::Db;
         break;
      case 8:
         step -= Pitch::Ab;
         break;
      case 9:
         step -= Pitch::Eb;
         break;
      case 10:
         step -= Pitch::Bb;
         break;
      case 11:
         step -= Pitch::F;
         break;
      default :
         throw std::runtime_error("NoteLabeler::setLabel() fifths is out of range");
      }
      while (step < 0) {
         step += 12;
      }
      while (12 <= step) {
         step -= 12;
      }
      label.setRelativePitch(static_cast<size_t>(step));
   }

   label.setKey(key.getFifths());
   label.setBeat(beat);

   {
      double t = 0.0;
      size_t d = 0;
      const DataList::const_iterator itrEnd(dataList.end());
      for (DataList::const_iterator itr(dataList.begin()); itrEnd != itr; ++itr) {
         size_t dur((*itr)->getNote().getDuration());
         d += dur;
         t += static_cast<double>(dur) / (*itr)->getTempo();
      }
      label.setTempo(static_cast<double>(d) / t);
   }

   label.setSyllableNum(children.size());

   {
      LabelPosition position;
      std::for_each(dataList.begin(), dataList.end(), PositionAdder(position));
      position.setCount(1);
      label.setLength(position);
   }

   if (measure) {
      label.setPositionInMeasure(measureIdx, measure->getMaxPosition());
   }

   if (phrase) {
      label.setPositionInPhrase(phraseIdx, phrase->getPosition());
   }

   label.setSlurFromPrev(inSlurFromPrev);
   label.setSlurToNext(inSlurToNext);
   label.setDynamics(dynamics);

   if (0 < nextAccent.getCount()) {
      label.setLengthToNextAccent(nextAccent);
   }
   if (0 < prevAccent.getCount()) {
      label.setLengthFromPrevAccent(prevAccent);
   }
   if (0 < nextStaccato.getCount()) {
      label.setLengthToNextStaccato(nextStaccato);
   }
   if (0 < prevStaccato.getCount()) {
      label.setLengthFromPrevStaccato(prevStaccato);
   }

   if (crescendo) {
      label.setPositionInCrescendo(crescendoIdx, crescendo->getPosition());
   }
   if (diminuendo) {
      label.setPositionInDiminuendo(diminuendoIdx, diminuendo->getPosition());
   }

   if (!isRest()) {
      if (hasPrevPitch) {
         label.setPitchDifferenceFromPrev(pitchDifferenceFromPrev);
      }
      if (hasNextPitch) {
         label.setPitchDifferenceToNext(pitchDifferenceToNext);
      }
   }

   if (prevPhrase) {
      label.setPrevPhraseInfo(prevPhrase->getSyllableNum(), prevPhrase->getPosition().getCount());
   }
   if (phrase) {
      label.setPhraseInfo(phrase->getSyllableNum(), phrase->getPosition().getCount());
   }
   if (nextPhrase) {
      label.setNextPhraseInfo(nextPhrase->getSyllableNum(), nextPhrase->getPosition().getCount());
   }
}

/*!
 add note

 @param note note
 @param tempo tempo
 */
void NoteLabeler::addNote(const Note& note, double tempo)
{
   NoteData* data(new NoteData(note, tempo));
   dataList.push_back(data);

   this->lyric = this->connectLyrics();

   // add position (count is 0)
   if (measure) {
      LabelPosition tmp(note.getDuration(), tempo);
      tmp.setCount(0);
      measure->addPosition(tmp);
   }
}

/*!
 get beat
 */
const Beat& NoteLabeler::getBeat() const
{
   return beat;
}

/*!
 get dynamics
 */
const Dynamics& NoteLabeler::getDynamics() const
{
   return dynamics;
}

/*!
 get key
 */
const Key& NoteLabeler::getKey() const
{
   return key;
}

/*!
 set measure
 */
void NoteLabeler::setMeasure(LabelMeasure* m)
{
   if (NULL != m) {
      measureIdx = m->getPosition();
      LabelPosition position;
      std::for_each(dataList.begin(), dataList.end(), PositionAdder(position));
      position.setCount(1);

      // add only count
      {
         LabelPosition tmp;
         tmp.setCount(1);
         m->addPosition(tmp);
      }

   } else {
      measureIdx = LabelPosition();
   }
   measure = m;
}

/*!
 set phrase
 */
void NoteLabeler::setPhrase(NoteGroup* ng)
{
   if (NULL != ng) {
      phraseIdx = ng->getPosition();
      LabelPosition position;
      std::for_each(dataList.begin(), dataList.end(), PositionAdder(position));
      position.setCount(1);
      ng->addPosition(position);
   } else {
      phraseIdx = LabelPosition();
   }
   phrase = ng;
}

/*!
 set crescendo
 */
void NoteLabeler::setCrescendo(NoteGroup* ng)
{
   if (NULL != ng) {
      crescendoIdx = ng->getPosition();
      LabelPosition position;
      std::for_each(dataList.begin(), dataList.end(), PositionAdder(position));
      position.setCount(1);
      ng->addPosition(position);
   } else {
      crescendoIdx = LabelPosition();
   }

   crescendo = ng;
}

/*!
 set diminuendo
 */
void NoteLabeler::setDiminuendo(NoteGroup* ng)
{
   if (NULL != ng) {
      diminuendoIdx = ng->getPosition();
      LabelPosition position;
      std::for_each(dataList.begin(), dataList.end(), PositionAdder(position));
      position.setCount(1);
      ng->addPosition(position);
   } else {
      diminuendoIdx = LabelPosition();
   }
   diminuendo = ng;
}

/*!
 set previous phrase
 */
void NoteLabeler::setPrevPhrase(const NoteGroup* ng)
{
   prevPhrase = ng;
}

/*!
 set next phrase
 */
void NoteLabeler::setNextPhrase(const NoteGroup* ng)
{
   nextPhrase = ng;
}

/*!
 get phrase
 */
const NoteGroup* NoteLabeler::getPhrase() const
{
   return phrase;
}

/*!
 get pitch
 */
Pitch NoteLabeler::getPitch() const
{
   if (dataList.empty()) {
      ERR_MSG("Empty note : " << this);
      throw std::runtime_error("NoteLabeler::getPitch() dataList is NULL");
   }
   return dataList.front()->getNote().getPitch();
}

/*!
 is slur start or not
 */
bool NoteLabeler::isSlurStart() const
{
   bool ret(false);

   DataList::const_iterator itr(dataList.begin());
   const DataList::const_iterator itrEnd(dataList.end());
   for (; itrEnd != itr; ++itr) {
      if ((*itr)->getNote().isSlurStop()) {
         ret = false;
      }
      if ((*itr)->getNote().isSlurStart()) {
         ret = true;
      }
   }

   return ret;
}

/*!
 is slur stop or not
 */
bool NoteLabeler::isSlurStop() const
{
   bool ret(false);

   DataList::const_iterator itr(dataList.begin());
   const DataList::const_iterator itrEnd(dataList.end());
   for (; itrEnd != itr; ++itr) {
      if ((*itr)->getNote().isSlurStop()) {
         ret = true;
      }
      if ((*itr)->getNote().isSlurStart()) {
         ret = false;
      }
   }

   return ret;
}

/*!
 set if this note is in slur from previous note or not
 */
void NoteLabeler::setInSlurFromPrev(bool b)
{
   inSlurFromPrev = b;
}

/*!
 set if this note is in slur to next note or not
 */
void NoteLabeler::setInSlurToNext(bool b)
{
   inSlurToNext = b;
}

/*!
 set previous pitch
 */
void NoteLabeler::setPrevPitch(const Pitch& p)
{
   pitchDifferenceFromPrev = calcPitchDifference(p, getPitch());
   hasPrevPitch = true;
}

/*!
 set next pitch
 */
void NoteLabeler::setNextPitch(const Pitch& p)
{
   pitchDifferenceToNext = calcPitchDifference(p, getPitch());
   hasNextPitch = true;
}

/*!
 get length
 */
LabelPosition NoteLabeler::getLength() const
{
   LabelPosition ret;
   DataList::const_iterator itr(dataList.begin());
   const DataList::const_iterator itrEnd(dataList.end());
   for (; itrEnd != itr; ++itr) {
      ret += (*itr)->getLength();
   }
   ret.setCount(1);
   return ret;
}

/*!
 set next accent position
 */
void NoteLabeler::setNextAccentPosition(const LabelPosition& p)
{
   nextAccent = p;
}

/*!
 set previous accent position
 */
void NoteLabeler::setPrevAccentPosition(const LabelPosition& p)
{
   prevAccent = p;
}

/*!
 set next staccato position
 */
void NoteLabeler::setNextStaccatoPosition(const LabelPosition& p)
{
   nextStaccato = p;
}

/*!
 set previous staccato position
 */
void NoteLabeler::setPrevStaccatoPosition(const LabelPosition& p)
{
   prevStaccato = p;
}

/*!
 get number of syllables
 */
size_t NoteLabeler::getSyllableNum() const
{
   return children.size();
}

/*!
 has accent or not
 */
bool NoteLabeler::hasAccent() const
{
   const DataList::const_iterator itrEnd(dataList.end());
   for (DataList::const_iterator itr(dataList.begin()); itrEnd != itr; ++itr) {
      if ((*itr)->getNote().hasAccent()) {
         return true;
      }
   }
   return false;
}

/*!
 has staccato or not
 */
bool NoteLabeler::hasStaccato() const
{
   const DataList::const_iterator itrEnd(dataList.end());
   for (DataList::const_iterator itr(dataList.begin()); itrEnd != itr; ++itr) {
      if ((*itr)->getNote().hasStaccato()) {
         return true;
      }
   }
   return false;
}

/*!
 has breath between this and next notes or not
 */
bool NoteLabeler::hasBreathToNext() const
{
   const DataList::const_iterator itrEnd(dataList.end());
   for (DataList::const_iterator itr(dataList.begin()); itrEnd != itr; ++itr) {
      if ((*itr)->getNote().hasBreathMark()) {
         return true;
      }
   }
   return false;
}

/*!
 set previous note (null -> this is the head note)
 */
void NoteLabeler::setPrevNote(const NoteLabeler* prev)
{
   this->prevNote = prev;
}

/*!
 set next note (null -> this is the tail note)
 */
void NoteLabeler::setNextNote(const NoteLabeler* next)
{
   this->nextNote = next;
}

/*!
 get measure index
 */
const LabelPosition& NoteLabeler::getMeasureIdx() const
{
   return measureIdx;
}


/*!
 set breath phoneme to the tail of the last syllable
 */
void NoteLabeler::setBreathPhoneme()
{
   if (children.empty()) {
      WARN_MSG("Cannot set breath phoneme : no syllables in a note");
      return;
   }

   List::reverse_iterator lastSyllableItr(children.rbegin());
   SyllableLabeler::List::const_iterator phonemeItr((*lastSyllableItr)->childBegin());

   if ((*lastSyllableItr)->childEnd() == phonemeItr) {
      WARN_MSG("Cannot set breath phoneme : no phonemes in a syllable");
      return;
   }

   PhonemeLabeler* br = new PhonemeLabeler(PhonemeInfo(PhonemeInfo::TYPE_BREAK, PhonemeLabeler::BREATH_PHONEME, (*phonemeItr)->getScoreFlag()));
   (*lastSyllableItr)->addChild(br);
}

/*!
 move to target
 */
void NoteLabeler::moveTo(NoteLabeler& target, size_t durationStartIdx)
{
   size_t residualSize(0);
   size_t totalDuration(0);
   const DataList::iterator itrEnd(dataList.end());
   for (DataList::iterator itr(dataList.begin()); itrEnd != itr; ++itr) {
      size_t dur = (*itr)->getNote().getDuration();

      if (durationStartIdx <= totalDuration) { // move
         target.addNote((*itr)->getNote(), (*itr)->getTempo());
      } else {
         ++residualSize;
         INT64 diff = static_cast<INT64>(totalDuration + dur) - durationStartIdx;
         if (0 < diff) { // cut note and move one-side
            (*itr)->note.setDuration(dur - diff);

            Note note((*itr)->getNote());
            note.setDuration(diff);
            target.addNote(note, (*itr)->getTempo());
         }
      }
      totalDuration += dur;
   }

   // cut
   std::for_each(dataList.begin() + residualSize, itrEnd, Deleter<NoteData>());
   dataList.resize(residualSize);
}

/*!
 get begin iterator of syllables
 */
NoteLabeler::List::const_iterator NoteLabeler::childBegin() const
{
   return children.begin();
}

/*!
 get end iterator of syllables
 */
NoteLabeler::List::const_iterator NoteLabeler::childEnd() const
{
   return children.end();
}

/*!
 constructor
 */
NoteLabeler::NoteData::NoteData(const Note& n, double t) : note(n), tempo(t)
{
}

/*!
 destructor
 */
NoteLabeler::NoteData::~NoteData()
{
}

/*!
 get note
 */
const Note& NoteLabeler::NoteData::getNote() const
{
   return note;
}

/*!
 get tempo
 */
double NoteLabeler::NoteData::getTempo() const
{
   return tempo;
}

/*!
 get length
 */
LabelPosition NoteLabeler::NoteData::getLength() const
{
   LabelPosition ret;
   ret.add(note.getDuration(), tempo);
   return ret;
}

/*!
 constructor
 */
NoteLabeler::PositionAdder::PositionAdder(LabelPosition& p) : position(p)
{
}

/*!
 copy constructor
 */
NoteLabeler::PositionAdder::PositionAdder(const PositionAdder& obj) : position(obj.position)
{
}

/*!
 destructor
 */
NoteLabeler::PositionAdder::~PositionAdder()
{
}

/*!
 ...
 */
void NoteLabeler::PositionAdder::operator()(NoteData* data)
{
   position.add(data->getNote().getDuration(), data->getTempo());
}

};  // namespace sinsy

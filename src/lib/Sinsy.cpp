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

#include <fstream>
#include "sinsy.h"
#include "util_log.h"
#include "Converter.h"
#include "TempScore.h"
#include "XmlReader.h"
#include "XmlWriter.h"
#include "InputFile.h"
#include "OutputFile.h"
#include "WritableStrStream.h"
#include "LabelStream.h"
#include "LabelStrings.h"
#include "LabelMaker.h"
#include "HtsEngine.h"
#include "SynthConditionImpl.h"
#include "ScorePosition.h"
#include "ScoreDoctor.h"
#include "util_score.h"

namespace sinsy
{

namespace
{
const std::string DEFAULT_LANGUAGES = "j";

class ScoreConverter : public IScoreWritable
{
public:
   //! constructor
   explicit ScoreConverter(IScore& s);

   //! destructor
   virtual ~ScoreConverter();

   //! set encoding
   virtual void setEncoding(const std::string& encoding);

   //! change tempo
   virtual void changeTempo(double tempo);

   //! change beat
   virtual void changeBeat(const Beat& beat);

   //! change dynamics
   virtual void changeDynamics(const Dynamics& dynamics);

   //! change key
   virtual void changeKey(const Key& key);

   //! change divisions (duration of a quarter note)
   virtual void changeDivisions(size_t d);

   //! start crescendo
   virtual void startCrescendo();

   //! start diminuendo
   virtual void startDiminuendo();

   //! stop crescendo
   virtual void stopCrescendo();

   //! stop diminuendo
   virtual void stopDiminuendo();

   //! add note
   virtual void addNote(const Note& note);

private:
   //! copy constructor (donot use)
   ScoreConverter(const ScoreConverter&);

   //! assignment operator (donot use)
   ScoreConverter& operator=(const ScoreConverter&);

   //! score
   IScore& score;

   //! divisions
   size_t divisions;
};

/*!
 constructor
 */
ScoreConverter::ScoreConverter(IScore& s) : score(s), divisions(BASE_DIVISIONS)
{
}

/*!
 destructor
 */
ScoreConverter::~ScoreConverter()
{
}

/*!
 set encoding
 */
void ScoreConverter::setEncoding(const std::string& encoding)
{
   if (!score.setEncoding(encoding)) {
      ERR_MSG("Cannot set encoding : " << encoding);
      throw std::invalid_argument("ScoreConverter::setEncoding()");
   }
}

/*!
 change tempo
 */
void ScoreConverter::changeTempo(double tempo)
{
   if (!score.addTempoMark(tempo)) {
      ERR_MSG("Cannot change tempo : " << tempo);
      throw std::invalid_argument("ScoreConverter::changeTempo()");
   }
}

/*!
 change beat
 */
void ScoreConverter::changeBeat(const Beat& beat)
{
   if (!score.addBeatMark(beat.getBeats(), beat.getBeatType())) {
      ERR_MSG("Cannot change beat : " << beat);
      throw std::invalid_argument("ScoreConverter::changeBeat()");
   }
}

/*!
 change dynamics
 */
void ScoreConverter::changeDynamics(const Dynamics& dynamics)
{
   SuddenDynamicsType d(SUDDENDYNAMICSTYPE_N);
   if (dynamics == Dynamics::PPPP) {
      d = SUDDENDYNAMICSTYPE_PPPP;
   } else if (dynamics == Dynamics::PPP) {
      d = SUDDENDYNAMICSTYPE_PPP;
   } else if (dynamics == Dynamics::PP) {
      d = SUDDENDYNAMICSTYPE_PP;
   } else if (dynamics == Dynamics::P) {
      d = SUDDENDYNAMICSTYPE_P;
   } else if (dynamics == Dynamics::MP) {
      d = SUDDENDYNAMICSTYPE_MP;
   } else if (dynamics == Dynamics::N) {
      d = SUDDENDYNAMICSTYPE_N;
   } else if (dynamics == Dynamics::MF) {
      d = SUDDENDYNAMICSTYPE_MF;
   } else if (dynamics == Dynamics::F) {
      d = SUDDENDYNAMICSTYPE_F;
   } else if (dynamics == Dynamics::FF) {
      d = SUDDENDYNAMICSTYPE_FF;
   } else if (dynamics == Dynamics::FFF) {
      d = SUDDENDYNAMICSTYPE_FFF;
   } else if (dynamics == Dynamics::FFFF) {
      d = SUDDENDYNAMICSTYPE_FFFF;
   } else { // fail safe
      ERR_MSG("Error in API " << FUNC_NAME(dynamics) << " : unknown type");
      throw std::invalid_argument("ScoreConverter::changeDynamics() unknown type");
   }
   if (!score.addSuddenDynamicsMark(d)) {
      ERR_MSG("Error in API " << FUNC_NAME(dynamics) << " : Cannot add sudden dynamics mark");
      throw std::invalid_argument("ScoreConverter::changeDynamics()");
   }
}

/*!
 change key
 */
void ScoreConverter::changeKey(const Key& key)
{
   const Mode& mode(key.getMode());
   ModeType m(MODETYPE_MAJOR);
   if (mode == Mode::MAJOR) {
      m = MODETYPE_MAJOR;
   } else if (mode == Mode::MINOR) {
      m = MODETYPE_MINOR;
   } else { // fail safe
      ERR_MSG("Cannot change key : " << key);
      throw std::invalid_argument("ScoreConverter::changeKey() unknown mode type");
   }
   if (!score.addKeyMark(m, key.getFifths())) {
      ERR_MSG("Cannot change key : " << key);
      throw std::invalid_argument("ScoreConverter::changeKey()");
   }
}

/*!
 change divisions (duration of a quarter note)
 */
void ScoreConverter::changeDivisions(size_t d)
{
   if (d <= 0) {
      ERR_MSG("Cannot change divisions : " << d);
      std::invalid_argument("ScoreConverter::changeDivisions()");
   }
   divisions = d;
}

/*!
 start crescendo
 */
void ScoreConverter::startCrescendo()
{
   if (!score.addGradualDynamicsMark(GRADUALDYNAMICSTYPE_CRESCENDO_BEGIN)) {
      ERR_MSG("Cannot start crescendo");
      throw std::invalid_argument("ScoreConverter::startCrescendo()");
   }
}

/*!
 start diminuendo
 */
void ScoreConverter::startDiminuendo()
{
   if (!score.addGradualDynamicsMark(GRADUALDYNAMICSTYPE_DIMINUENDO_BEGIN)) {
      ERR_MSG("Cannot start diminuendo");
      throw std::invalid_argument("ScoreConverter::startDiminuendo()");
   }
}

/*!
 stop crescendo
 */
void ScoreConverter::stopCrescendo()
{
   if (!score.addGradualDynamicsMark(GRADUALDYNAMICSTYPE_CRESCENDO_END)) {
      ERR_MSG("Cannot stop crescendo");
      throw std::invalid_argument("ScoreConverter::stopCrescendo()");
   }
}

/*!
 stop diminuendo
 */
void ScoreConverter::stopDiminuendo()
{
   if (!score.addGradualDynamicsMark(GRADUALDYNAMICSTYPE_DIMINUENDO_END)) {
      ERR_MSG("Cannot stop diminuendo");
      throw std::invalid_argument("ScoreConverter::stopDiminuendo()");
   }
}

/*!
 add note
 */
void ScoreConverter::addNote(const Note& note)
{
   if (note.isRest()) { // rest
      if (!score.addRest(note.getDuration() * BASE_DIVISIONS / divisions)) {
         ERR_MSG("Cannot add rest : " << note);
         throw std::invalid_argument("ScoreConverter::addNote()");
      }
   } else { // pitch
      Pitch pitch(note.getPitch());
      TieType tieType(TIETYPE_NONE);
      if (note.isTieStart()) {
         tieType = TIETYPE_BEGIN;
      } else if (note.isTieStop()) {
         tieType = TIETYPE_END;
      }
      SlurType slurType(SLURTYPE_NONE);
      if (note.isSlurStart()) {
         slurType = SLURTYPE_BEGIN;
      } else if (note.isSlurStop()) {
         slurType = SLURTYPE_END;
      }
      SyllabicType syllabicType(SYLLABICTYPE_SINGLE);
      Syllabic syllabic(note.getSyllabic());
      if (Syllabic::BEGIN == syllabic) {
         syllabicType = SYLLABICTYPE_BEGIN;
      } else if (Syllabic::MIDDLE == syllabic) {
         syllabicType = SYLLABICTYPE_MIDDLE;
      } else if (Syllabic::END == syllabic) {
         syllabicType = SYLLABICTYPE_END;
      }

      if (!score.addNote(note.getDuration() * BASE_DIVISIONS / divisions, note.getLyric(), pitch.getOctave() * 12 + pitch.getStep(), note.hasAccent(), note.hasStaccato(), tieType, slurType, syllabicType, note.hasBreathMark())) {
         ERR_MSG("Cannot add note : " << note);
         throw std::invalid_argument("ScoreConverter::addNote()");
      }
   }
}

};

/*!
 constructor
 */
SynthCondition::SynthCondition() : impl(NULL)
{
   this->impl = new SynthConditionImpl();
}

/*!
 destructor
 */
SynthCondition::~SynthCondition()
{
   delete this->impl;
}

/*!
 set play flag
 */
void SynthCondition::setPlayFlag()
{
   this->impl->setPlayFlag();
}

/*!
 unset play flag
 */
void SynthCondition::unsetPlayFlag()
{
   this->impl->unsetPlayFlag();
}

/*!
 set save file path
 */
void SynthCondition::setSaveFilePath(const std::string& filePath)
{
   this->impl->setSaveFilePath(filePath);
}

/*!
 unset save file path
 */
void SynthCondition::unsetSaveFilePath()
{
   this->impl->unsetSaveFilePath();
}

/*!
 set waveform buffer
 */
void SynthCondition::setWaveformBuffer(std::vector<double>& waveform)
{
   this->impl->setWaveformBuffer(waveform);
}

/*!
 unset waveform buffer
 */
void SynthCondition::unsetWaveformBuffer()
{
   this->impl->unsetWaveformBuffer();
}


class SinsyImpl : public IScoreWriter
{
public:
   //! constructor
   SinsyImpl() {}

   //! destructor
   virtual ~SinsyImpl() {}

   //! set languages
   bool setLanguages(const std::string& languages, const std::string& dirPath) {
      return converter.setLanguages(languages, dirPath);
   }

   //! load voice files
   bool loadVoices(const std::vector<std::string>& voices) {
      return engine.load(voices);
   }

   //! set encoding
   void setEncoding(const std::string& encoding) {
      score.setEncoding(encoding);
   }

   //! add key mark
   void changeKey(const Key& key) {
      score.changeKey(key);
   }

   //! change beat : default beat mark is 4/4
   void changeBeat(const Beat& beat) {
      score.changeBeat(beat);
   }

   //! change tempo : default tempo is 100bps
   void changeTempo(double tempo) {
      score.changeTempo(tempo);
   }

   //! change dynamics (sudden changes)
   void changeDynamics(const Dynamics& dynamics) {
      score.changeDynamics(dynamics);
   }

   //! start crescendo
   void startCrescendo() {
      score.startCrescendo();
   }

   //! stop crescendo
   void stopCrescendo() {
      score.stopCrescendo();
   }

   //! start diminuendo
   void startDiminuendo() {
      score.startDiminuendo();
   }

   //! stop diminuendo
   void stopDiminuendo() {
      score.stopDiminuendo();
   }

   //! add note to end of score
   void addNote(const Note& note) {
      score.addNote(note);
   }

   //! write score to given IScoreWritable object
   void write(IScoreWritable& maker) const {
      maker << score;
   }

   //! set alpha for synthesis
   bool setAlpha(double alpha) {
      return engine.setAlpha(alpha);
   }

   //! set volume for synthesis
   bool setVolume(double volume) {
      return engine.setVolume(volume);
   }

   //! set interpolation weight for synthesis
   bool setInterpolationWeight(size_t index, double weight) {
      return engine.setInterpolationWeight(index, weight);
   }

   LabelStrings* createLabelData(bool monophoneFlag, int overwriteEnableFlag, int timeFlag) {
      LabelMaker labelMaker(converter);
      labelMaker << score;
      labelMaker.fix();
      LabelStrings *label = new LabelStrings;
      labelMaker.outputLabel(*label, monophoneFlag, overwriteEnableFlag, timeFlag);

      return label;
   }

   //! synthesize
   bool synthesize(SynthConditionImpl& condition) {
      LabelMaker labelMaker(converter);
      labelMaker << score;
      labelMaker.fix();
      LabelStrings label;

      labelMaker.outputLabel(label, false, 1, 2);

      return engine.synthesize(label, condition);
   }

   //! stop synthesizing
   void stop() {
      engine.stop();
   }

   //! reset stop flag
   void resetStopFlag() {
      engine.resetStopFlag();
   }

   //! clear score
   void clearScore() {
      score.clear();
   }

   //! load score from MusicXML
   bool loadScoreFromMusicXML(InputFile& xml) {
      XmlReader xmlReader;
      if (!xmlReader.readXml(xml)) {
         ERR_MSG("Cannot parse Xml file");
         return false;
      }
      score << xmlReader;
      return true;
   }

   //! save score to MusicXML
   bool saveScoreToMusicXML(const std::string& xml, XmlWriter::Clef clef) {
      XmlWriter xmlWriter;
      xmlWriter.setClef(clef);
      xmlWriter << score;

      OutputFile outputFile(xml);
      if (!outputFile.isValid()) {
         ERR_MSG("Cannot open Xml file");
         return false;
      }

      WritableStrStream stream(outputFile);
      if (!xmlWriter.writeXml(stream)) {
         ERR_MSG("Cannot write Xml file");
         return false;
      }
      return true;
   }

   size_t get_sampling_frequency() {
      return engine.get_sampling_frequency();
   }

private:
   //! copy constructor (donot use)
   SinsyImpl(const SinsyImpl&);

   //! assignment operator (donot use)
   SinsyImpl& operator=(const SinsyImpl&);

   //! score
   ScoreDoctor score;

   //! converter
   Converter converter;

   //! hts_engine API
   HtsEngine engine;
};

/*!
 constructor
 */
Sinsy::Sinsy() : impl(NULL)
{
   try {
      impl = new SinsyImpl();
   } catch (const std::bad_alloc& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      delete(impl); // fail safe
   }
}

/*!
 destructor
 */
Sinsy::~Sinsy()
{
   delete(impl);
}

/*!
 set languages
 */
bool Sinsy::setLanguages(const std::string& languages, const std::string& dirPath)
{
   try {
      if (!impl->setLanguages(languages, dirPath)) {
         return false;
      }
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME(languages << ", " << dirPath) << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 load voices
 */
bool Sinsy::loadVoices(const std::vector<std::string>& voices)
{
   try {
      if (!impl->loadVoices(voices)) {
         return false;
      }
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 set encoding
 */
bool Sinsy::setEncoding(const std::string& encoding)
{
   try {
      impl->setEncoding(encoding);
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME(encoding) << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 add beat mark
 */
bool Sinsy::addBeatMark(size_t beats, size_t beatType)
{
   if (0 == beats) {
      ERR_MSG("Wrong beats (== 0) in API " << FUNC_NAME(beats << ", " << beatType));
      return false;
   }
   if (0 == beatType) {
      ERR_MSG("Wrong beat type (== 0) in API " << FUNC_NAME(beats << ", " << beatType));
      return false;
   }
   try {
      impl->changeBeat(Beat(beats, beatType));
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME(beats << ", " << beatType) << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 add tempo mark
 */
bool Sinsy::addTempoMark(double tempo)
{
   if (tempo <= 0.0) {
      return false;
   }
   try {
      impl->changeTempo(tempo);
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME(tempo) << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 add sudden dynamics mark
 */
bool Sinsy::addSuddenDynamicsMark(SuddenDynamicsType suddenDynamicsType)
{
   try {
      switch (suddenDynamicsType) {
      case SUDDENDYNAMICSTYPE_PPPP :
         impl->changeDynamics(Dynamics::PPPP);
         break;
      case SUDDENDYNAMICSTYPE_PPP :
         impl->changeDynamics(Dynamics::PPP);
         break;
      case SUDDENDYNAMICSTYPE_PP :
         impl->changeDynamics(Dynamics::PP);
         break;
      case SUDDENDYNAMICSTYPE_P :
         impl->changeDynamics(Dynamics::P);
         break;
      case SUDDENDYNAMICSTYPE_MP :
         impl->changeDynamics(Dynamics::MP);
         break;
      case SUDDENDYNAMICSTYPE_N :
         impl->changeDynamics(Dynamics::N);
         break;
      case SUDDENDYNAMICSTYPE_MF :
         impl->changeDynamics(Dynamics::MF);
         break;
      case SUDDENDYNAMICSTYPE_F :
         impl->changeDynamics(Dynamics::F);
         break;
      case SUDDENDYNAMICSTYPE_FF :
         impl->changeDynamics(Dynamics::FF);
         break;
      case SUDDENDYNAMICSTYPE_FFF :
         impl->changeDynamics(Dynamics::FFF);
         break;
      case SUDDENDYNAMICSTYPE_FFFF :
         impl->changeDynamics(Dynamics::FFFF);
         break;
      default :
         ERR_MSG("Unknown sudden dynamics type in API " << FUNC_NAME(suddenDynamicsType));
         return false;
      }
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME(suddenDynamicsType) << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 add gradual dynamics mark
 */
bool Sinsy::addGradualDynamicsMark(GradualDynamicsType gradualDynamicsType)
{
   try {
      switch (gradualDynamicsType) {
      case GRADUALDYNAMICSTYPE_CRESCENDO_BEGIN :
         impl->startCrescendo();
         break;
      case GRADUALDYNAMICSTYPE_CRESCENDO_END :
         impl->stopCrescendo();
         break;
      case GRADUALDYNAMICSTYPE_DIMINUENDO_BEGIN :
         impl->startDiminuendo();
         break;
      case GRADUALDYNAMICSTYPE_DIMINUENDO_END :
         impl->stopDiminuendo();
         break;
      default :
         ERR_MSG("Unknown gradual dynamics type in API " << FUNC_NAME(gradualDynamicsType));
         return false;
      }
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME(gradualDynamicsType) << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 add key mark
 */
bool Sinsy::addKeyMark(ModeType modeType, int fifths)
{
   try {
      Mode mode;
      switch (modeType) {
      case MODETYPE_MAJOR :
         mode = Mode::MAJOR;
         break;
      case MODETYPE_MINOR :
         mode = Mode::MINOR;
         break;
      default :
         ERR_MSG("Unknown mode type in API " << FUNC_NAME(modeType << ", " << fifths));
         return false;
      }
      impl->changeKey(Key(mode, fifths));
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME(modeType << ", " << fifths) << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 add note
 */
bool Sinsy::addNote(size_t duration, const std::string& lyric, size_t pitch, bool accent, bool staccato, TieType tieType, SlurType slurType, SyllabicType syllabicType, bool breath)
{
   if (duration <= 0) {
      ERR_MSG("Error in API " << FUNC_NAME("") << " : duration(" << duration << ") <= 0");
      return false;
   }
   try {
      Note note;
      note.setRest(false);
      note.setDuration(duration);
      note.setLyric(lyric);
      note.setPitch(Pitch(pitch % 12, pitch / 12));
      note.setAccent(accent);
      note.setStaccato(staccato);
      note.setBreathMark(breath);

      switch (tieType) {
      case TIETYPE_NONE :
         break;
      case TIETYPE_BEGIN :
         note.setTieStart(true);
         break;
      case TIETYPE_END :
         note.setTieStop(true);
         break;
      default :
         ERR_MSG("Error in API " << FUNC_NAME("") << " : unknown tie type : " << tieType);
         return false;
      }

      switch (slurType) {
      case SLURTYPE_NONE :
         break;
      case SLURTYPE_BEGIN :
         note.setSlurStart(true);
         break;
      case SLURTYPE_END :
         note.setSlurStop(true);
         break;
      default :
         ERR_MSG("Error in API " << FUNC_NAME("") << " : unknown slur type : " << slurType);
         return false;
      }

      switch (syllabicType) {
      case SYLLABICTYPE_SINGLE :
         note.setSyllabic(Syllabic::SINGLE);
         break;
      case SYLLABICTYPE_BEGIN :
         note.setSyllabic(Syllabic::BEGIN);
         break;
      case SYLLABICTYPE_MIDDLE :
         note.setSyllabic(Syllabic::MIDDLE);
         break;
      case SYLLABICTYPE_END :
         note.setSyllabic(Syllabic::END);
         break;
      default :
         ERR_MSG("Error in API " << FUNC_NAME("") << " : unknown syllabic type : " << syllabicType);
         return false;
      }
      impl->addNote(note);
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 add rest
 */
bool Sinsy::addRest(size_t duration)
{
   if (duration <= 0) {
      ERR_MSG("Error in API " << FUNC_NAME(duration) << " : duration <= 0");
      return false;
   }
   try {
      Note note;
      note.setRest(true);
      note.setDuration(duration);
      impl->addNote(note);
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME(duration) << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 push score data to s
 */
bool Sinsy::toScore(IScore& s) const
{
   try {
      ScoreConverter cs(s);
      cs << *impl;
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 set alpha
 */
bool Sinsy::setAlpha(double alpha)
{
   return impl->setAlpha(alpha);
}

/*!
 set volume
 */
bool Sinsy::setVolume(double volume)
{
   return impl->setVolume(volume);
}

/*!
 set interpolation weight
 */
bool Sinsy::setInterpolationWeight(size_t index, double weight)
{
   return impl->setInterpolationWeight(index, weight);
}

LabelStrings* Sinsy::createLabelData(bool monophoneFlag, int overwriteEnableFlag, int timeFlag) {
   return impl->createLabelData(monophoneFlag, overwriteEnableFlag, timeFlag);
}

/*!
 synthesize
 */
bool Sinsy::synthesize(SynthCondition& condition)
{
   try {
      if (!impl->synthesize(*condition.impl)) {
         return false;
      }
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

bool Sinsy::synthesize(SynthCondition* condition)
{
   return synthesize(*condition);
}

/*!
 stop
 */
bool Sinsy::stop()
{
   try {
      impl->stop();
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 reset stop flag
 */
bool Sinsy::resetStopFlag()
{
   try {
      impl->resetStopFlag();
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 clear score
 */
bool Sinsy::clearScore()
{
   try {
      impl->clearScore();
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 load score from MusicXML
 */
bool Sinsy::loadScoreFromMusicXML(const std::string& xml)
{
   try {
      InputFile xmlFile(xml);
      if(!xmlFile.isValid()) {
         ERR_MSG("Cannot open Xml file");
         return false;
      }
      if (!impl->loadScoreFromMusicXML(xmlFile)) {
         return false;
      }
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

/*!
 save score to MusicXML
 */
bool Sinsy::saveScoreToMusicXML(const std::string& xml, ClefType clefType)
{
   try {
      XmlWriter::Clef clef(XmlWriter::CLEF_DEFAULT);
      switch (clefType) {
      case CLEFTYPE_DEFAULT :
         clef = XmlWriter::CLEF_DEFAULT;
         break;
      case CLEFTYPE_G :
         clef = XmlWriter::CLEF_G;
         break;
      case CLEFTYPE_F :
         clef = XmlWriter::CLEF_F;
         break;
      case CLEFTYPE_C :
         clef = XmlWriter::CLEF_C;
         break;
      default :
         ERR_MSG("Error in API " << FUNC_NAME("") << " : unknown clef type : " << clefType);
         break;
      }

      if (!impl->saveScoreToMusicXML(xml, clef)) {
         return false;
      }
   } catch (const std::exception& ex) {
      ERR_MSG("Exception in API " << FUNC_NAME("") << " : " << ex.what());
      return false;
   }
   return true;
}

size_t Sinsy::get_sampling_frequency() {
   return impl->get_sampling_frequency();
}

};  // namespace sinsy

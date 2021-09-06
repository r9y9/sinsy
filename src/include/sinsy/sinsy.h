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

#ifndef SINSY_H_
#define SINSY_H_

#ifdef __cplusplus

#include <string>
#include <vector>

#include "LabelStrings.h"

namespace sinsy
{
typedef size_t SuddenDynamicsType;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_PPPP = 0;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_PPP  = 1;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_PP   = 2;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_P    = 3;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_MP   = 4;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_N    = 5;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_MF   = 6;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_F    = 7;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_FF   = 8;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_FFF  = 9;
const SuddenDynamicsType SUDDENDYNAMICSTYPE_FFFF = 10;

typedef size_t GradualDynamicsType;
const GradualDynamicsType GRADUALDYNAMICSTYPE_CRESCENDO_BEGIN  = 0;
const GradualDynamicsType GRADUALDYNAMICSTYPE_CRESCENDO_END    = 1;
const GradualDynamicsType GRADUALDYNAMICSTYPE_DIMINUENDO_BEGIN = 2;
const GradualDynamicsType GRADUALDYNAMICSTYPE_DIMINUENDO_END   = 3;

typedef size_t TieType;
const TieType TIETYPE_NONE  = 0;
const TieType TIETYPE_BEGIN = 1;
const TieType TIETYPE_END   = 2;

typedef size_t SlurType;
const SlurType SLURTYPE_NONE   = 0;
const SlurType SLURTYPE_BEGIN  = 1;
const SlurType SLURTYPE_END    = 2;

typedef size_t SyllabicType;
const SyllabicType SYLLABICTYPE_SINGLE = 0;
const SyllabicType SYLLABICTYPE_BEGIN  = 1;
const SyllabicType SYLLABICTYPE_MIDDLE = 2;
const SyllabicType SYLLABICTYPE_END    = 3;

typedef size_t ModeType;
const ModeType MODETYPE_MAJOR = 0;
const ModeType MODETYPE_MINOR = 1;

typedef size_t ClefType;
const ClefType CLEFTYPE_DEFAULT = 0;
const ClefType CLEFTYPE_G = 1;
const ClefType CLEFTYPE_F = 2;
const ClefType CLEFTYPE_C = 3;


class SynthConditionImpl;
class SinsyImpl;

class IScore
{
public:
   //! destructor
   virtual ~IScore() {}

   //! set encoding
   virtual bool setEncoding(const std::string& encoding) = 0;

   //! add key mark
   virtual bool addKeyMark(ModeType modeType, int fifths) = 0;

   //! add beat mark (beats/beatType) to end of score: default beat mark is 4/4
   virtual bool addBeatMark(size_t beats, size_t beatType) = 0;

   //! add tempo mark to end of score: default tempo is 100bps
   virtual bool addTempoMark(double tempo) = 0;

   //! add dynamics mark (sudden changes) to end of score
   virtual bool addSuddenDynamicsMark(SuddenDynamicsType suddenDynamicsType) = 0;

   //! add dynamics mark (gradual changes) to end of score
   virtual bool addGradualDynamicsMark(GradualDynamicsType gradualDynamicsType) = 0;

   //! add note to end of score
   virtual bool addNote(size_t duration, const std::string& lyric, size_t pitch, bool accent, bool staccato, TieType tieType, SlurType slurType, SyllabicType syllabicType, bool breath = false) = 0;

   //! add rest to end of score
   virtual bool addRest(size_t duration) = 0;
};

class SynthCondition
{
public:
   //! constructor
   SynthCondition();

   //! destructor
   virtual ~SynthCondition();

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
   SynthCondition(const SynthCondition&);

   //! assignment operator (donot use)
   SynthCondition& operator=(const SynthCondition&);

   //! implementation
   SynthConditionImpl* impl;

   friend class Sinsy;
};

class Sinsy
{
public:
   //! constructor
   Sinsy();

   //! destructor
   virtual ~Sinsy();

   //! set languages
   bool setLanguages(const std::string& languages, const std::string& configs);

   //! load voice files
   bool loadVoices(const std::vector<std::string>& voices);

   //! set encoding
   virtual bool setEncoding(const std::string& encoding);

   //! add key mark
   virtual bool addKeyMark(ModeType modeType, int fifths);

   //! add beat mark (beats/beattype) to end of score: default beat mark is 4/4
   virtual bool addBeatMark(size_t beats, size_t beattype);

   //! add tempo mark to end of score: default tempo is 100bps
   virtual bool addTempoMark(double tempo);

   //! add dynamics mark (sudden changes) to end of score
   virtual bool addSuddenDynamicsMark(SuddenDynamicsType suddenDynamicsType);

   //! add dynamics mark (gradual changes) to end of score
   virtual bool addGradualDynamicsMark(GradualDynamicsType gradualDynamicsType);

   //! add note to end of score
   virtual bool addNote(size_t duration, const std::string& lyric, size_t pitch, bool accent, bool staccato, TieType tieType, SlurType slurType, SyllabicType syllabicType, bool breath = false);

   //! add rest to end of score
   virtual bool addRest(size_t duration);

   //! set score to given IScore object
   bool toScore(IScore&) const;

   //! set alpha for synthesis
   bool setAlpha(double alpha);

   //! set volume for synthesis
   bool setVolume(double volume);

   //! set interpolation weight for synthesis
   bool setInterpolationWeight(size_t index, double weight);

   LabelStrings* createLabelData(bool monophoneFlag, int overwriteEnableFlag, int timeFlag);

   //! synthesize
   bool synthesize(SynthCondition& consition);

   bool synthesize(SynthCondition* consition);

   //! stop synthesizing
   bool stop();

   //! reset stop flag
   bool resetStopFlag();

   //! clear score
   bool clearScore();

   //! load score from MusicXML
   bool loadScoreFromMusicXML(const std::string& xml);

   //! save score to MusicXML
   bool saveScoreToMusicXML(const std::string& xml, ClefType clefType = CLEFTYPE_DEFAULT);

   size_t get_sampling_frequency();

private:
   //! copy constructor (donot use)
   Sinsy(const Sinsy&);

   //! assignment operator (donot use)
   Sinsy& operator=(const Sinsy&);

   //! implementation
   SinsyImpl* impl;
};

};

#endif /* __cplusplus */

#endif /* SINSY_H_ */

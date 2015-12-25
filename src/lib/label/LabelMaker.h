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

#ifndef SINSY_LABEL_MAKER_H_
#define SINSY_LABEL_MAKER_H_

#include <string>
#include <vector>
#include <deque>
#include "IScoreWritable.h"
#include "Beat.h"
#include "Dynamics.h"
#include "Mode.h"
#include "IConvertable.h"
#include "Converter.h"
#include "IConf.h"
#include "TempScore.h"
#include "NoteLabeler.h"
#include "SyllableLabeler.h"
#include "PhonemeLabeler.h"
#include "LabelData.h"
#include "ILabelOutput.h"
#include "SynthConditionImpl.h"

namespace sinsy
{

class LabelMaker : public IScoreWritable
{
public:
   //! constructor
   explicit LabelMaker(Converter& converter, bool sepRests = true);

   //! destructor
   virtual ~LabelMaker();

   //! set encoding
   virtual void setEncoding(const std::string& e);

   //! change tempo
   virtual void changeTempo(double t);

   //! change beat
   virtual void changeBeat(const Beat& b);

   //! change dynamics
   virtual void changeDynamics(const Dynamics& d);

   //! change key
   virtual void changeKey(const Key& k);

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

   //! fix
   void fix();

   //! output label
   void outputLabel(ILabelOutput& output, bool monophoneFlag = false, int overwriteEnableFlag = 0, int timeFlag = 0) const;

private:
   //! copy constructor (donot use)
   LabelMaker(const LabelMaker&);

   //! assignment operator (donot use)
   LabelMaker& operator=(const LabelMaker&);

   //! get last measure
   LabelMeasure* getLastMeasure();

   //! converter
   Converter& converter;

   //! encoding of lyrics
   std::string encoding;

   //! ...
   bool separateWholeNoteRests;

   //! is fixed or not
   bool isFixed;

   //! temporary score
   TempScore tempScore;

   //! tempo
   double tempo;

   //! key
   Key key;

   //! beat
   Beat beat;

   //! dynamics
   Dynamics dynamics;

   //! number of syllables
   size_t syllableNum;

   //! in tie
   bool inTie;

   //! in crescendo
   bool inCrescendo;

   //! in diminuendo
   bool inDiminuendo;

   //! position of measure
   LabelPosition measurePosition;

   //! duration of residual measure
   int residualMeasureDuration;

   typedef std::deque<LabelMeasure*> MeasureQue;

   //! stock of measures
   MeasureQue stockMeasures;

   typedef std::vector<LabelMeasure*> MeasureList;

   //! list of measures
   MeasureList measureList;

   typedef std::vector<NoteGroup*> NoteGroupList;

   //! list of phrases
   NoteGroupList phraseList;

   //! list of crescendos
   NoteGroupList crescendoList;

   //! list of diminuendos
   NoteGroupList diminuendoList;

   typedef std::vector<NoteLabeler*> NoteList;

   //! list of notes
   NoteList noteList;

   typedef NoteList::const_iterator ConstNoteItr;

   //! move to previous note
   bool moveToPrevNote(ConstNoteItr& nItr, bool skipRests) const;

   //! move to next note
   bool moveToNextNote(ConstNoteItr& nItr, bool skipRests) const;

   typedef NoteLabeler::List::const_iterator ConstSyllableItr;

   //! move to previous syllable
   bool moveToPrevSyllable(ConstNoteItr& nItr, ConstSyllableItr& sItr, bool skipRests) const;

   //! move to next syllable
   bool moveToNextSyllable(ConstNoteItr& nItr, ConstSyllableItr& sItr, bool skipRests) const;

   typedef SyllableLabeler::List::const_iterator ConstPhonemeItr;

   //! move to previous phoneme
   bool moveToPrevPhoneme(ConstNoteItr& nItr, ConstSyllableItr& sItr, ConstPhonemeItr& pItr, bool skipRests) const;

   //! move to next phoneme
   bool moveToNextPhoneme(ConstNoteItr& nItr, ConstSyllableItr& sItr, ConstPhonemeItr& pItr, bool skipRests) const;

   //! set label data
   void setLabelData(LabelData& label, const ConstNoteItr& noteItr, const ConstSyllableItr& syllableItr, const ConstPhonemeItr& phonemeItr, int overwriteEnableFlag, int timeFlag) const;

   //! apply stocks
   void applyStocks();
};

};

#endif // SINSY_LABEL_MAKER_H_

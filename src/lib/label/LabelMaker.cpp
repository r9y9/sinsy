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
#include <sstream>
#include <limits>
#include "LabelMaker.h"
#include "util_string.h"
#include "util_log.h"
#include "util_score.h"
#include "Note.h"
#include "Deleter.h"
#include "NoteGroup.h"
#include "ILabelOutput.h"
#include "LabelData.h"
#include "LabelPosition.h"
#include "LabelMeasure.h"
#include "ScorePosition.h"
#include "util_score.h"

namespace sinsy
{

namespace
{
const std::string DEFAULT_ENCODING = "utf-8";

class Copier
{
public:
   //! constructor
   Copier(IConf::ConvertableList& c);

   //! destructor
   virtual ~Copier();

   //! ...
   void operator()(NoteLabeler* n);

private:
   //! list of IConvertable
   IConf::ConvertableList& cList;
};

/*!
 constructor
 */
Copier::Copier(IConf::ConvertableList& c) : cList(c)
{
}

/*!
 destructor
 */
Copier::~Copier()
{
}

/*!
 ...
 */
void Copier::operator()(NoteLabeler* n)
{
   cList.push_back(n);
}

class PhonemeLabel : public IPhonemeLabel
{
public:
   //! constructor
   PhonemeLabel(LabelData& ld, int d) : labelData(ld), diff(d) {}

   //! destructor
   virtual ~PhonemeLabel() {}

   //! set type
   virtual void setType(const std::string& value) {}

   //! set name
   virtual void setName(const std::string& value) {
      if (!value.empty()) {
         labelData.set('p', 4 + diff, value);
      }
   }

   //! set flag
   virtual void setFlag(const std::string& value) {
      if (!value.empty()) {
         labelData.set('p', 9 + diff, value);
      }
   }

   //! set position in syllable
   virtual void setPositionInSyllable(size_t idx, size_t max) {}

   //! set count from previous vowel
   virtual void setCountFromPrevVowel(size_t count) {}

   //! set count to next vowel
   virtual void setCountToNextVowel(size_t count) {}

protected:
   //! label data
   LabelData& labelData;

   //! diff from current phoneme
   int diff;
};

class PrevPrevPhonemeLabel : public PhonemeLabel
{
public:
   //! constructor
   PrevPrevPhonemeLabel(LabelData& ld) : PhonemeLabel(ld, -2) {}

   //! destructor
   virtual ~PrevPrevPhonemeLabel() {}
};

class PrevPhonemeLabel : public PhonemeLabel
{
public:
   //! constructor
   PrevPhonemeLabel(LabelData& ld) : PhonemeLabel(ld, -1) {}

   //! destructor
   virtual ~PrevPhonemeLabel() {}
};

class CurrentPhonemeLabel : public PhonemeLabel
{
public:
   //! constructor
   CurrentPhonemeLabel(LabelData& ld) : PhonemeLabel(ld, 0) {}

   //! destructor
   virtual ~CurrentPhonemeLabel() {}

   //! set type
   virtual void setType(const std::string& value) {
      if (!value.empty()) {
         labelData.set('p', 1, value);
      }
   }

   //! set position in syllable
   virtual void setPositionInSyllable(size_t idx, size_t max) {
      labelData.set('p', 12, std::min<size_t>(idx + 1, 9));
      labelData.set('p', 13, std::min<size_t>(max - idx, 9));
   }

   //! set count from previous vowel
   virtual void setCountFromPrevVowel(size_t count) {
      if (0 < count) {
         labelData.set('p', 14, std::min<size_t>(count, 9));
      }
   }

   //! set count to next vowel
   virtual void setCountToNextVowel(size_t count) {
      if (0 < count) {
         labelData.set('p', 15, std::min<size_t>(count, 9));
      }
   }
};

class NextPhonemeLabel : public PhonemeLabel
{
public:
   //! constructor
   NextPhonemeLabel(LabelData& ld) : PhonemeLabel(ld, 1) {}

   //! destructor
   virtual ~NextPhonemeLabel() {}
};

class NextNextPhonemeLabel : public PhonemeLabel
{
public:

   //! constructor
   NextNextPhonemeLabel(LabelData& ld) : PhonemeLabel(ld, 2) {}

   //! destructor
   virtual ~NextNextPhonemeLabel() {}
};

class SyllableLabel : public ISyllableLabel
{
public:
   //! constructor
   SyllableLabel(LabelData& ld, char c) : labelData(ld), category(c) {}

   //! destructor
   virtual ~SyllableLabel() {}

   //! set phoneme number
   virtual void setPhonemeNum(size_t value) {
      labelData.set(category, 1, std::min<size_t>(value, 9));
   }

   //! set position in note
   virtual void setPositionInNote(size_t idx, size_t max) {
      labelData.set(category, 2, std::min<size_t>(idx + 1, 9));
      labelData.set(category, 3, std::min<size_t>(max - idx, 9));
   }

   //! set language
   virtual void setLanguage(const std::string& value) {
      labelData.set(category, 4, value);
   }

   //! set language dependent information
   virtual void setLangDependentInfo(const std::string& value) {
      labelData.set(category, 5, value);
   }

protected:
   //! label data
   LabelData& labelData;

   //! category
   const char category;
};

class PrevSyllableLabel : public SyllableLabel
{
public:
   //! constructor
   PrevSyllableLabel(LabelData& ld) : SyllableLabel(ld, 'a') {}

   //! destructor
   virtual ~PrevSyllableLabel() {}
};

class CurrentSyllableLabel : public SyllableLabel
{
public:
   //! constructor
   CurrentSyllableLabel(LabelData& ld) : SyllableLabel(ld, 'b') {}

   //! destructor
   virtual ~CurrentSyllableLabel() {}
};

class NextSyllableLabel : public SyllableLabel
{
public:
   //! constructor
   NextSyllableLabel(LabelData& ld) : SyllableLabel(ld, 'c') {}

   //! destructor
   virtual ~NextSyllableLabel() {}
};

class _NoteLabel : public INoteLabel
{
public:
   //! constructor
   _NoteLabel(LabelData& ld, char c) : labelData(ld), category(c) {}

   //! destructor
   virtual ~_NoteLabel() {}

   //! set absolute pitch
   virtual void setAbsolutePitch(const Pitch& value) {
      labelData.set(category, 1, value);
   }

   //! set relative pitch
   virtual void setRelativePitch(size_t value) {
      labelData.set(category, 2, value);
   }

   //! set key
   virtual void setKey(size_t value) {
      labelData.set(category, 3, value);
   }

   //! set beat
   virtual void setBeat(const Beat& value) {
      labelData.set(category, 4, value);
   }

   //! set tempo
   virtual void setTempo(double value);

   //! set syllable number
   virtual void setSyllableNum(size_t value);

   //! set length
   virtual void setLength(const LabelPosition& value);

   //! set position in measure
   virtual void setPositionInMeasure(const LabelPosition&, const LabelPosition&) {}

   //! set position in phrase
   virtual void setPositionInPhrase(const LabelPosition&, const LabelPosition&) {}

   //! set slur from previous note
   virtual void setSlurFromPrev(bool) {}

   //! set slur to next note
   virtual void setSlurToNext(bool) {}

   //! set dynamics
   virtual void setDynamics(const Dynamics&) {}

   //! set length from previous accent
   virtual void setLengthFromPrevAccent(const LabelPosition&) {}

   //! set length to next accent
   virtual void setLengthToNextAccent(const LabelPosition&) {}

   //! set length from previous staccato
   virtual void setLengthFromPrevStaccato(const LabelPosition&) {}

   //! set length to next staccato
   virtual void setLengthToNextStaccato(const LabelPosition&) {}

   //! set position in creshendo
   virtual void setPositionInCrescendo(const LabelPosition&, const LabelPosition&) {}

   //! set position in diminuendo
   virtual void setPositionInDiminuendo(const LabelPosition&, const LabelPosition&) {}

   //! set pitch difference from previous note
   virtual void setPitchDifferenceFromPrev(int) {}

   //! set pitch difference to next note
   virtual void setPitchDifferenceToNext(int) {}

   //! set breath between previous and this notes
   virtual void setBreathFromPrev(bool) {}

   //! set breath between this and next notes
   virtual void setBreathToNext(bool) {}

   //! set prevous phrase information
   virtual void setPrevPhraseInfo(size_t, size_t) {}

   //! set phrase information
   virtual void setPhraseInfo(size_t, size_t) {}

   //! set next phrase information
   virtual void setNextPhraseInfo(size_t, size_t) {}

protected:
   //! label data
   LabelData& labelData;

   //! category
   const char category;
};

/*!
 set tempo
 */
void _NoteLabel::setTempo(double value)
{
   if (value <= 0) {
      throw std::range_error("_NoteLabel::setTempo()");
   }
   size_t bpm(static_cast<size_t>(value + 0.5));
   labelData.set(category, 5, std::min<size_t>(bpm, 199));
}

/*!
 set syllable number
 */
void _NoteLabel::setSyllableNum(size_t value)
{
   labelData.set(category, 6, std::min<size_t>(value, 3));
}

/*!
 set length
 */
void _NoteLabel::setLength(const LabelPosition& value)
{
   {
      size_t time(static_cast<size_t>(value.getTime() * 100));
      labelData.set(category, 7, std::min<size_t>(time, 499));
   }
   {
      if (value.getPoint() < 0) {
         throw std::range_error("setLength() point is out of range");
      }
      size_t point(static_cast<size_t>(value.getPoint()));
      labelData.set(category, 8, std::min<size_t>(point, 199));
   }
}

class PrevNoteLabel : public _NoteLabel
{
public:
   //! constructor
   PrevNoteLabel(LabelData& ld) : _NoteLabel(ld, 'd') {}

   //! destructor
   virtual ~PrevNoteLabel() {}
};

class NoteLabel : public _NoteLabel
{
public:
   //! constructor
   NoteLabel(LabelData& ld) : _NoteLabel(ld, 'e') {}

   //! destructor
   virtual ~NoteLabel() {}

   //! set position in measure
   virtual void setPositionInMeasure(const LabelPosition& idx, const LabelPosition& max);

   //! set position in phrase
   virtual void setPositionInPhrase(const LabelPosition& idx, const LabelPosition& max);

   //! set slur from previous note
   virtual void setSlurFromPrev(bool value) {
      labelData.set(category, 26, value);
   }

   //! set slur to next note
   virtual void setSlurToNext(bool value) {
      labelData.set(category, 27, value);
   }

   //! set dynamics
   virtual void setDynamics(const Dynamics& value) {
      labelData.set(category, 28, value);
   }

   //! set length to next accent
   virtual void setLengthToNextAccent(const LabelPosition& value);

   //! set length from previous accent
   virtual void setLengthFromPrevAccent(const LabelPosition& value);

   //! set length to next staccato
   virtual void setLengthToNextStaccato(const LabelPosition& value);

   //! set length from provious staccato
   virtual void setLengthFromPrevStaccato(const LabelPosition& value);

   //! set position in crescendo
   virtual void setPositionInCrescendo(const LabelPosition& idx, const LabelPosition& max);

   //! set position in diminuendo
   virtual void setPositionInDiminuendo(const LabelPosition& idx, const LabelPosition& max);

   //! set pitch difference from previous note
   virtual void setPitchDifferenceFromPrev(int value);

   //! set pitch difference to next note
   virtual void setPitchDifferenceToNext(int value);

   //! set breath from previous note
   virtual void setBreathFromPrev(bool value) {
      labelData.set(category, 59, value);
   }

   //! set breath to next note
   virtual void setBreathToNext(bool value) {
      labelData.set(category, 60, value);
   }

   //! set prevous phrase information
   virtual void setPrevPhraseInfo(size_t syllableNum, size_t noteNum) {
      labelData.set('g', 1, syllableNum);
      labelData.set('g', 2, noteNum);
   }

   //! set phrase information
   virtual void setPhraseInfo(size_t syllableNum, size_t noteNum) {
      labelData.set('h', 1, syllableNum);
      labelData.set('h', 2, noteNum);
   }

   //! set next phrase information
   virtual void setNextPhraseInfo(size_t syllableNum, size_t noteNum) {
      labelData.set('i', 1, syllableNum);
      labelData.set('i', 2, noteNum);
   }
};

/*!
 set position in measure
 */
void NoteLabel::setPositionInMeasure(const LabelPosition& idx, const LabelPosition& max)
{
   LabelPosition diff(max - idx);
   {
      if (idx.getCount() < 0) {
         throw std::range_error("setPositionInMeasure() count is out of range");
      }
      size_t count(static_cast<size_t>(idx.getCount()) + 1);
      labelData.set(category, 10, std::min<size_t>(count, 49));
   }
   {
      if (diff.getCount() < 0) {
         throw std::range_error("setPositionInMeasure() count is out of range");
      }
      size_t count(static_cast<size_t>(diff.getCount()));
      labelData.set(category, 11, std::min<size_t>(count, 49));
   }
   {
      if (idx.getTime() < 0.0) {
         throw std::range_error("setPositionInMeasure() time is out of range");
      }
      size_t time(static_cast<size_t>(idx.getTime() * 10));
      labelData.set(category, 12, std::min<size_t>(time, 49));
   }
   {
      if (diff.getTime() < 0.0) {
         WARN_MSG("Wrong position in a measure : position[" << idx << " / " << max << "] (probably calculation error)");
      }
      size_t time(static_cast<size_t>(diff.getTime() * 10));
      labelData.set(category, 13, std::min<size_t>(time, 49));
   }
   {
      if (idx.getPoint() < 0) {
         throw std::range_error("setPositionInMeasure() point is out of range");
      }
      size_t point(static_cast<size_t>(idx.getPoint()));
      labelData.set(category, 14, std::min<size_t>(point, 96));
   }
   {
      if (diff.getPoint() < 0) {
         throw std::range_error("setPositionInMeasure() point is out of range");
      }
      size_t point(static_cast<size_t>(diff.getPoint()));
      labelData.set(category, 15, std::min<size_t>(point, 96));
   }

   size_t persent(static_cast<size_t>(idx.getTime() * 100 / max.getTime()));
   labelData.set(category, 16, persent);
   labelData.set(category, 17, 100 - persent);
}

/*!
 set position in phrase
 */
void NoteLabel::setPositionInPhrase(const LabelPosition& idx, const LabelPosition& max)
{
   LabelPosition diff(max - idx);
   {
      if (idx.getCount() < 0) {
         throw std::range_error("setPositionInPhrase() count is out of range");
      }
      size_t count(static_cast<size_t>(idx.getCount()) + 1);
      labelData.set(category, 18, std::min<size_t>(count, 49));
   }
   {
      if (diff.getCount() < 0) {
         throw std::range_error("setPositionInPhrase() count is out of range");
      }
      size_t count(static_cast<size_t>(diff.getCount()));
      labelData.set(category, 19, std::min<size_t>(count, 49));
   }
   {
      if (idx.getTime() < 0.0) {
         throw std::range_error("setPositionInPhrase() time is out of range");
      }
      size_t time(static_cast<size_t>(idx.getTime() * 10));
      labelData.set(category, 20, std::min<size_t>(time, 199));
   }
   {
      if (diff.getTime() < 0.0) {
         WARN_MSG("Wrong position in a phrase : position[" << idx << " / " << max << "] (probably calculation error)");
      }
      size_t time(static_cast<size_t>(diff.getTime() * 10));
      labelData.set(category, 21, std::min<size_t>(time, 199));
   }
   {
      if (idx.getPoint() < 0) {
         throw std::range_error("setPositionInPhrase() point is out of range");
      }
      size_t point(static_cast<size_t>(idx.getPoint()));
      labelData.set(category, 22, std::min<size_t>(point, 499));
   }
   {
      if (diff.getPoint() < 0) {
         throw std::range_error("setPositionInPhrase() point is out of range");
      }
      size_t point(static_cast<size_t>(diff.getPoint()));
      labelData.set(category, 23, std::min<size_t>(point, 499));
   }

   size_t persent(static_cast<size_t>(idx.getTime() * 100 / max.getTime()));
   labelData.set(category, 24, persent);
   labelData.set(category, 25, 100 - persent);
}

/*!
 set length to next accent
 */
void NoteLabel::setLengthToNextAccent(const LabelPosition& value)
{
   {
      if (value.getCount() < 0) {
         throw std::range_error("setLengthToNextAccent() count is out of range");
      }
      size_t count(static_cast<size_t>(value.getCount()));
      labelData.set(category, 29, std::min<size_t>(count, 9));
   }
   {
      if (value.getTime() < 0.0) {
         throw std::range_error("setLengthToNextAccent() time is out of range");
      }
      size_t time(static_cast<size_t>(value.getTime() * 10));
      labelData.set(category, 31, std::min<size_t>(time, 99));
   }
   {
      if (value.getPoint() < 0) {
         throw std::range_error("setLengthToNextAccent() point is out of range");
      }
      size_t point(static_cast<size_t>(value.getPoint()));
      labelData.set(category, 33, std::min<size_t>(point, 99));
   }
}

/*!
 set length from previous accent
 */
void NoteLabel::setLengthFromPrevAccent(const LabelPosition& value)
{
   {
      if (value.getCount() < 0) {
         throw std::range_error("setLengthFromPrevAccent() count is out of range");
      }
      size_t count(static_cast<size_t>(value.getCount()));
      labelData.set(category, 30, std::min<size_t>(count, 9));
   }
   {
      if (value.getTime() < 0.0) {
         throw std::range_error("setLengthFromPrevAccent() time is out of range");
      }
      size_t time(static_cast<size_t>(value.getTime() * 10));
      labelData.set(category, 32, std::min<size_t>(time, 99));
   }
   {
      if (value.getPoint() < 0) {
         throw std::range_error("setLengthFromPrevAccent() point is out of range");
      }
      size_t point(static_cast<size_t>(value.getPoint()));
      labelData.set(category, 34, std::min<size_t>(point, 99));
   }
}

/*!
 set length to next staccato
 */
void NoteLabel::setLengthToNextStaccato(const LabelPosition& value)
{
   {
      if (value.getCount() < 0) {
         throw std::range_error("setLengthToNextStaccato() count is out of range");
      }
      size_t count(static_cast<size_t>(value.getCount()));
      labelData.set(category, 35, std::min<size_t>(count, 9));
   }
   {
      if (value.getTime() < 0.0) {
         throw std::range_error("setLengthToNextStaccato() time is out of range");
      }
      size_t time(static_cast<size_t>(value.getTime() * 10));
      labelData.set(category, 37, std::min<size_t>(time, 99));
   }
   {
      if (value.getPoint() < 0) {
         throw std::range_error("setLengthToNextStaccato() point is out of range");
      }
      size_t point(static_cast<size_t>(value.getPoint()));
      labelData.set(category, 39, std::min<size_t>(point, 99));
   }
}

/*!
 set length from previous staccato
 */
void NoteLabel::setLengthFromPrevStaccato(const LabelPosition& value)
{
   {
      if (value.getCount() < 0) {
         throw std::range_error("setLengthFromPrevStaccato() count is out of range");
      }
      size_t count(static_cast<size_t>(value.getCount()));
      labelData.set(category, 36, std::min<size_t>(count, 9));
   }
   {
      if (value.getTime() < 0.0) {
         throw std::range_error("setLengthFromPrevStaccato() time is out of range");
      }
      size_t time(static_cast<size_t>(value.getTime() * 10));
      labelData.set(category, 38, std::min<size_t>(time, 99));
   }
   {
      if (value.getPoint() < 0) {
         throw std::range_error("setLengthFromPrevStaccato() point is out of range");
      }
      size_t point(static_cast<size_t>(value.getPoint()));
      labelData.set(category, 40, std::min<size_t>(point, 99));
   }
}

/*!
 set position in crescendo
 */
void NoteLabel::setPositionInCrescendo(const LabelPosition& idx, const LabelPosition& max)
{
   LabelPosition diff(max - idx);
   {
      if (idx.getCount() < 0) {
         throw std::range_error("setPositionInCrescendo() count is out of range");
      }
      size_t count(static_cast<size_t>(idx.getCount()) + 1);
      labelData.set(category, 41, std::min<size_t>(count, 49));
   }
   {
      if (diff.getCount() < 0) {
         throw std::range_error("setPositionInCrescendo() count is out of range");
      }
      size_t count(static_cast<size_t>(diff.getCount()));
      labelData.set(category, 42, std::min<size_t>(count, 49));
   }
   {
      if (idx.getTime() < 0.0) {
         throw std::range_error("setPositionInCrescendo() time is out of range");
      }
      size_t time(static_cast<size_t>(idx.getTime() * 10));
      labelData.set(category, 43, std::min<size_t>(time, 99));
   }
   {
      if (diff.getTime() < 0.0) {
         WARN_MSG("Wrong position in a crescendo : position[" << idx << " / " << max << "] (probably calculation error)");
      }
      size_t time(static_cast<size_t>(diff.getTime() * 10));
      labelData.set(category, 44, std::min<size_t>(time, 99));
   }
   {
      if (idx.getPoint() < 0) {
         throw std::range_error("setPositionInCrescendo() point is out of range");
      }
      size_t point(static_cast<size_t>(idx.getPoint()));
      labelData.set(category, 45, std::min<size_t>(point, 499));
   }
   {
      if (diff.getPoint() < 0) {
         throw std::range_error("setPositionInCrescendo() point is out of range");
      }
      size_t point(static_cast<size_t>(diff.getPoint()));
      labelData.set(category, 46, std::min<size_t>(point, 499));
   }

   size_t persent(static_cast<size_t>(idx.getTime() * 100 / max.getTime()));
   labelData.set(category, 47, persent);
   labelData.set(category, 48, 100 - persent);
}

/*!
 set position in diminundo
 */
void NoteLabel::setPositionInDiminuendo(const LabelPosition& idx, const LabelPosition& max)
{
   LabelPosition diff(max - idx);
   {
      if (idx.getCount() < 0) {
         throw std::range_error("setPositionInDiminuendo() count is out of range");
      }
      size_t count(static_cast<size_t>(idx.getCount()) + 1);
      labelData.set(category, 49, std::min<size_t>(count, 49));
   }
   {
      if (diff.getCount() < 0) {
         throw std::range_error("setPositionInDiminuendo() count is out of range");
      }
      size_t count(static_cast<size_t>(diff.getCount()));
      labelData.set(category, 50, std::min<size_t>(count, 49));
   }
   {
      if (idx.getTime() < 0.0) {
         throw std::range_error("setPositionInDiminuendo() time is out of range");
      }
      size_t time(static_cast<size_t>(idx.getTime() * 10));
      labelData.set(category, 51, std::min<size_t>(time, 99));
   }
   {
      if (diff.getTime() < 0.0) {
         WARN_MSG("Wrong position in a diminuendo : position[" << idx << " / " << max << "] (probably calculation error)");
      }
      size_t time(static_cast<size_t>(diff.getTime() * 10));
      labelData.set(category, 52, std::min<size_t>(time, 99));
   }
   {
      if (idx.getPoint() < 0) {
         throw std::range_error("setPositionInDiminuendo() point is out of range");
      }
      size_t point(static_cast<size_t>(idx.getPoint()));
      labelData.set(category, 53, std::min<size_t>(point, 499));
   }
   {
      if (diff.getPoint() < 0) {
         throw std::range_error("setPositionInDiminuendo() point is out of range");
      }
      size_t point(static_cast<size_t>(diff.getPoint()));
      labelData.set(category, 54, std::min<size_t>(point, 499));
   }

   size_t persent(static_cast<size_t>(idx.getTime() * 100 / max.getTime()));
   labelData.set(category, 55, persent);
   labelData.set(category, 56, 100 - persent);
}

/*!
 set pitch difference from previous note
 */
void NoteLabel::setPitchDifferenceFromPrev(int value)
{
   std::ostringstream oss;
   if (value < 0) {
      oss << "m";
      value *= -1;
   } else {
      oss << "p";
   }
   oss << value;
   labelData.set(category, 57, oss.str());
}

/*!
 set pitch difference to next
 */
void NoteLabel::setPitchDifferenceToNext(int value)
{
   std::ostringstream oss;
   if (value < 0) {
      oss << "m";
      value *= -1;
   } else {
      oss << "p";
   }
   oss << value;
   labelData.set(category, 58, oss.str());
}


class NextNoteLabel : public _NoteLabel
{
public:
   //! constructor
   NextNoteLabel(LabelData& ld) : _NoteLabel(ld, 'f') {}

   //! destructor
   virtual ~NextNoteLabel() {}
};

}; // namespace

/*!
 constructor
 */
LabelMaker::LabelMaker(Converter& c, bool sepRests) :
   converter(c), encoding(DEFAULT_ENCODING), separateWholeNoteRests(sepRests),
   isFixed(false), tempo(DEFAULT_TEMPO), syllableNum(0),
   inTie(false), inCrescendo(false), inDiminuendo(false), residualMeasureDuration(0)
{
}

/*!
 destructor
 */
LabelMaker::~LabelMaker()
{
   std::for_each(noteList.begin(), noteList.end(), Deleter<NoteLabeler>());
   std::for_each(measureList.begin(), measureList.end(), Deleter<LabelMeasure>());
   std::for_each(phraseList.begin(), phraseList.end(), Deleter<NoteGroup>());
   std::for_each(crescendoList.begin(), crescendoList.end(), Deleter<NoteGroup>());
   std::for_each(diminuendoList.begin(), diminuendoList.end(), Deleter<NoteGroup>());
}

/*!
 set encoding
*/
void LabelMaker::setEncoding(const std::string& e)
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::setEncoding() already fixed");
   }
   this->encoding = e;
}

/*!
 change tempo
*/
void LabelMaker::changeTempo(double t)
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::changeTempo() already fixed");
   }
   this->tempo = t;
}

/*!
 change beat
*/
void LabelMaker::changeBeat(const Beat& b)
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::changeBeat() already fixed");
   }
   this->beat = b;
}

/*!
 change dynamics
*/
void LabelMaker::changeDynamics(const Dynamics& d)
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::changeDynamics() already fixed");
   }
   this->dynamics = d;
}

/*!
 change key
*/
void LabelMaker::changeKey(const Key& k)
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::changeKey() already fixed");
   }
   this->key = k;
}

/*!
 start crescendo
*/
void LabelMaker::startCrescendo()
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::startCrescendo() already fixed");
   }
   if (inTie) {
      tempScore.startCrescendo();
      return;
   }

   if (inCrescendo) {
      WARN_MSG("Cannot start a crescendo : already in a crescendo");
      return;
   }

   crescendoList.push_back(new NoteGroup());
   inCrescendo = true;
}

/*!
 start diminuendo
*/
void LabelMaker::startDiminuendo()
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::startDiminuendo() already fixed");
   }
   if (inTie) {
      tempScore.startDiminuendo();
      return;
   }

   if (inDiminuendo) {
      WARN_MSG("Cannot start a diminuendo : already in a diminuendo");
      return;
   }

   diminuendoList.push_back(new NoteGroup());
   inDiminuendo = true;
}

/*!
 stop crescendo
*/
void LabelMaker::stopCrescendo()
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::stopCrescendo() already fixed");
   }
   if (inTie) {
      tempScore.stopCrescendo();
      return;
   }
   inCrescendo = false;
}

/*!
 stop diminuendo
*/
void LabelMaker::stopDiminuendo()
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::stopDiminuendo() already fixed");
   }
   if (inTie) {
      tempScore.stopDiminuendo();
      return;
   }
   inDiminuendo = false;
}

/*!
 add note
 */
void LabelMaker::addNote(const Note& note)
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::addNote() already fixed");
   }

   // if the first note is not rest, add a rest at first
   if (!note.isRest() && measureList.empty())  {
      WARN_MSG("The first note is not rest");
      Note restNote;
      restNote.setRest(true);
      restNote.setDuration(getMeasureDuration(beat));
      addNote(restNote);
   }

   if (inTie && ((noteList.back()->isRest() != note.isRest())
                 || (!noteList.back()->isRest() && (noteList.back()->getPitch() != note.getPitch())))) {
      inTie = false;

      if (inCrescendo) {
         noteList.back()->setCrescendo(crescendoList.back());
      }
      if (inDiminuendo) {
         noteList.back()->setDiminuendo(diminuendoList.back());
      }

      applyStocks();

      // for log
      if (note.isRest()) {
         WARN_MSG("Rest cannot be tied");
      } else if (!noteList.back()->isRest() && (noteList.back()->getPitch() != note.getPitch())) {
         WARN_MSG("Notes that have different pitches cannot be tied");
      }
   }

   {
      size_t duration(note.getDuration());
      if (static_cast<size_t>(std::numeric_limits<int>::max()) < duration) {
         throw std::runtime_error("LabelMaker::addNote() duration is larger than max of int");
      }
      while (residualMeasureDuration < static_cast<int>(duration)) {
         if (0 < residualMeasureDuration) {
            measurePosition.add(residualMeasureDuration, tempo);
            getLastMeasure()->setMaxPosition(measurePosition);
            if (static_cast<int>(duration) < residualMeasureDuration) {
               throw std::runtime_error("LabelMaker::addNote() duration is smaller than residual measure duration");
            }
            duration -= residualMeasureDuration;
            if (static_cast<size_t>(std::numeric_limits<int>::max()) < duration) {
               throw std::runtime_error("LabelMaker::addNote() duration is larger than max of int");
            }
         }
         size_t measureIndex(measureList.size() + stockMeasures.size());
         stockMeasures.push_back(new LabelMeasure(this->beat));
         stockMeasures.back()->setIndex(measureIndex);
         if ((0 == residualMeasureDuration && !inTie)) {
            applyStocks();
         }
         residualMeasureDuration = getMeasureDuration(beat);
         measurePosition = LabelPosition(0, tempo);
      }
      measurePosition.add(duration, tempo);
      getLastMeasure()->setMaxPosition(measurePosition);
      residualMeasureDuration -= duration;
   }

   if (!inTie) {
      NoteLabeler* noteLabeler(new NoteLabeler(beat, dynamics, key));
      noteLabeler->setMeasure(measureList.back());
      noteList.push_back(noteLabeler);
   }

   if (note.isRest()) {
      inTie = true;
   } else {
      if (note.isTieStop()) {
         inTie = false;
      }
      if (note.isTieStart()) {
         inTie = true;
      }
   }

   noteList.back()->addNote(note, tempo);

   if (!inTie) {
      applyStocks();

      if (inCrescendo) {
         noteList.back()->setCrescendo(crescendoList.back());
      }
      if (inDiminuendo) {
         noteList.back()->setDiminuendo(diminuendoList.back());
      }
   }
}

/*!
 fix
*/
void LabelMaker::fix()
{
   if (isFixed) {
      throw std::runtime_error("LabelMaker::fix() already fixed");
   }

   // add last rest
   {
      // add rest to fix measure
      if (0 != residualMeasureDuration) {
         WARN_MSG("Length of notes in the last measure is not just");
         int dur(residualMeasureDuration);
         while (dur < 0) {
            dur += getMeasureDuration(beat);
         }
         Note restNote;
         restNote.setRest(true);
         restNote.setDuration(static_cast<size_t>(dur));
         addNote(restNote);
         // if the last note is not rest, add a rest at last
      } else if (!noteList.empty() && !noteList.back()->isRest()) {
         WARN_MSG("The last note is not rest");
         Note restNote;
         restNote.setRest(true);
         restNote.setDuration(getMeasureDuration(beat));
         addNote(restNote);
      }
      inTie = false;
      applyStocks();
   }

   {
      bool inSlur(false);
      NoteLabeler* lastPitch(NULL);
      syllableNum = 0;
      const NoteList::iterator itrBegin(noteList.begin());
      const NoteList::iterator itrEnd(noteList.end());
      for (NoteList::iterator itr(itrBegin); itrEnd != itr; ++itr) {
         // phrase
         {
            if (!(*itr)->isRest()) {
               if ((itr == itrBegin) || (*(itr - 1))->isRest() || (*(itr - 1))->hasBreathToNext()) {
                  phraseList.push_back(new NoteGroup());
               }
               (*itr)->setPhrase(phraseList.back());
            }
         }
         // next and prev notes
         {
            if (itrBegin != itr) {
               (*itr)->setPrevNote(*(itr - 1));
               (*(itr - 1))->setNextNote(*itr);
            }
         }

         // slur
         {
            (*itr)->setInSlurFromPrev(inSlur);
            if ((*itr)->isSlurStop()) {
               inSlur = false;
            }
            if ((*itr)->isSlurStart()) {
               inSlur = true;
            }
            (*itr)->setInSlurToNext(inSlur);
         }
         // pitch difference
         {
            if (!(*itr)->isRest()) {
               if (lastPitch) {
                  (*itr)->setPrevPitch(lastPitch->getPitch());
                  lastPitch->setNextPitch((*itr)->getPitch());
               }
               lastPitch = *itr;
            }
         }
      }
   }

   // accent & staccato (next)
   {
      bool hasAccent(false);
      bool hasStaccato(false);
      LabelPosition accentPos;
      LabelPosition staccatoPos;
      const NoteList::reverse_iterator itrEnd(noteList.rend());
      for (NoteList::reverse_iterator itr(noteList.rbegin()); itrEnd != itr; ++itr) {
         if ((*itr)->isRest()) {
            accentPos = LabelPosition();
            staccatoPos = LabelPosition();
            hasAccent = false;
            hasStaccato = false;
         } else {
            LabelPosition length((*itr)->getLength());
            if (hasAccent) {
               accentPos += length;
               (*itr)->setNextAccentPosition(accentPos);
            }
            if ((*itr)->hasAccent()) {
               accentPos = LabelPosition();
               hasAccent = true;
            }
            if (hasStaccato) {
               staccatoPos += length;
               (*itr)->setNextStaccatoPosition(staccatoPos);
            }
            if ((*itr)->hasStaccato()) {
               staccatoPos = LabelPosition();
               hasStaccato = true;
            }
         }
      }
   }

   // accent & staccato (prev)
   {
      bool hasAccent(false);
      bool hasStaccato(false);
      LabelPosition accentPos;
      LabelPosition staccatoPos;
      const NoteList::iterator itrEnd(noteList.end());
      for (NoteList::iterator itr(noteList.begin()); itrEnd != itr; ++itr) {
         if ((*itr)->isRest()) {
            accentPos = LabelPosition();
            staccatoPos = LabelPosition();
            hasAccent = false;
            hasStaccato = false;
         } else {
            LabelPosition length((*itr)->getLength());
            if (hasAccent) {
               (*itr)->setPrevAccentPosition(accentPos);
               accentPos += length;
            }
            if ((*itr)->hasAccent()) {
               accentPos = length;
               hasAccent = true;
            }
            if (hasStaccato) {
               (*itr)->setPrevStaccatoPosition(staccatoPos);
               staccatoPos += length;
            }
            if ((*itr)->hasStaccato()) {
               staccatoPos = length;
               hasStaccato = true;
            }
         }
      }
   }

   // set prev phrase
   {
      const NoteGroup* prevPhrase(NULL);
      const NoteList::iterator itrBegin(noteList.begin());
      const NoteList::iterator itrEnd(noteList.end());
      for (NoteList::iterator itr(itrBegin); itrEnd != itr; ++itr) {
         const NoteGroup* p((*itr)->getPhrase());
         const NoteGroup* pOld((itrBegin == itr) ? NULL : (*(itr - 1))->getPhrase());
         if (NULL != pOld) {
            if (pOld != p) {
               prevPhrase = pOld;
            }
         }
         (*itr)->setPrevPhrase(prevPhrase);
      }
   }

   // set next phrase
   {
      const NoteGroup* nextPhrase(NULL);
      const NoteList::reverse_iterator itrBegin(noteList.rbegin());
      const NoteList::reverse_iterator itrEnd(noteList.rend());
      for (NoteList::reverse_iterator itr(itrBegin); itrEnd != itr; ++itr) {
         const NoteGroup* p((*itr)->getPhrase());
         const NoteGroup* pOld((itrBegin == itr) ? NULL : (*(itr - 1))->getPhrase());
         if (NULL != pOld) {
            if (pOld != p) {
               nextPhrase = pOld;
            }
         }
         (*itr)->setNextPhrase(nextPhrase);
      }
   }

   // convert
   {
      IConf::ConvertableList cList;
      cList.reserve(noteList.size());
      std::for_each(noteList.begin(), noteList.end(), Copier(cList));
      converter.convert(encoding, cList.begin(), cList.end());
   }

   // breath
   {
      const NoteList::iterator itrBegin(noteList.begin());
      const NoteList::iterator itrEnd(noteList.end());
      for (NoteList::iterator itr(itrBegin); itrEnd != itr; ++itr) {
         if (!(*itr)->isRest() && (*itr)->hasBreathToNext()) { // note has breath symbol
            if ((itrEnd != itr + 1) && !(*(itr + 1))->isRest()) { // next note is not rest
               (*itr)->setBreathPhoneme();
            }
         }
      }
   }

   // set positions
   {
      const NoteList::iterator itrBegin(noteList.begin());
      const NoteList::iterator itrEnd(noteList.end());
      for (NoteList::iterator itr(itrBegin); itrEnd != itr; ++itr) {
         (*itr)->setPositions();
      }
   }

   // count syllables
   {
      syllableNum = 0;
      const NoteList::iterator itrEnd(noteList.end());
      for (NoteList::iterator itr(noteList.begin()); itrEnd != itr; ++itr) {
         syllableNum += (*itr)->getSyllableNum();
      }
   }

   isFixed = true;
}

/*!
 output label
*/
void LabelMaker::outputLabel(ILabelOutput& output, bool monophoneFlag, int overwriteEnableFlag, int timeFlag) const
{
   if ((overwriteEnableFlag < 0) || (2 < overwriteEnableFlag)) {
      throw std::out_of_range("LabelMaker::outputLabel() overwriteEnableFlag is out of range [0...2]");
   }
   if ((timeFlag < 0) || (2 < timeFlag)) {
      throw std::out_of_range("LabelMaker::outputLabel() timeFlag is out of range [0...2]");
   }

   double time(0.0);

   // Note
   const NoteList::const_iterator nItrBegin(noteList.begin());
   const NoteList::const_iterator nItrEnd(noteList.end());
   for (NoteList::const_iterator nItr(nItrBegin) ; nItrEnd != nItr; ++nItr) {

      double beginTime(0.0);

      // Syllable
      const NoteLabeler::List::const_iterator sItrBegin((*nItr)->childBegin());
      const NoteLabeler::List::const_iterator sItrEnd((*nItr)->childEnd());
      for (NoteLabeler::List::const_iterator sItr(sItrBegin) ; sItrEnd != sItr; ++sItr) {

         // Phoneme
         const SyllableLabeler::List::const_iterator pItrBegin((*sItr)->childBegin());
         const SyllableLabeler::List::const_iterator pItrEnd((*sItr)->childEnd());
         for (SyllableLabeler::List::const_iterator pItr(pItrBegin); pItrEnd != pItr; ++pItr) {
            LabelData labelData;
            labelData.setMonophoneFlag(monophoneFlag);

            if (0 != timeFlag) {
               labelData.setOutputTimeFlag(true);
               // begin time
               if ((sItrBegin == sItr) && (pItrBegin == pItr)) {
                  labelData.setBeginTime(time);
                  beginTime = time;
                  time += (*nItr)->getLength().getTime();
               } else if (1 == timeFlag) {
                  labelData.setBeginTime(beginTime);
               }

               // end time
               if ((sItrEnd == (sItr + 1)) && (pItrEnd == (pItr + 1))) {
                  labelData.setEndTime(time);
               } else if (1 == timeFlag) {
                  labelData.setEndTime(time);
               }
            } else {
               labelData.setOutputTimeFlag(false);
            }

            setLabelData(labelData, nItr, sItr, pItr, overwriteEnableFlag, timeFlag);

            std::ostringstream oss;
            oss << labelData;
            output.output(oss.str());

         }
      }
   }
}

/*!
 get last measure
*/
LabelMeasure* LabelMaker::getLastMeasure()
{
   if (!stockMeasures.empty()) {
      return stockMeasures.back();
   }
   if (!measureList.empty()) {
      return measureList.back();
   }
   return NULL;
}

/*!
 @internal

 move to previous note
*/
bool LabelMaker::moveToPrevNote(ConstNoteItr& nItr, bool skipRests) const
{
   for ( ; ; ) {
      if (noteList.begin() == nItr) {
         return false;
      }
      --nItr;
      if (!skipRests || !(*nItr)->isRest()) {
         break;
      }
   }
   return true;
}

/*!
 @internal

 move to next note
*/
bool LabelMaker::moveToNextNote(ConstNoteItr& nItr, bool skipRests) const
{
   for ( ; ; ) {
      if (noteList.end() == nItr + 1) {
         return false;
      }
      ++nItr;
      if (!skipRests || !(*nItr)->isRest()) {
         break;
      }
   }
   return true;
}

/*!
 @internal

 move to previous syllable
*/
bool LabelMaker::moveToPrevSyllable(ConstNoteItr& nItr, ConstSyllableItr& sItr, bool skipRests) const
{
   if ((*nItr)->childBegin() == sItr) {
      if (false == moveToPrevNote(nItr, skipRests)) {
         return false;
      }
      sItr = (*nItr)->childEnd() - 1;
   } else {
      --sItr;
   }
   return true;
}

/*!
 @internal

 move to next syllable
*/
bool LabelMaker::moveToNextSyllable(ConstNoteItr& nItr, ConstSyllableItr& sItr, bool skipRests) const
{
   if ((*nItr)->childEnd() - 1 == sItr) {
      if (false == moveToNextNote(nItr, skipRests)) {
         return false;
      }
      sItr = (*nItr)->childBegin();
   } else {
      ++sItr;
   }
   return true;
}

/*!
 @internal

 move to previous phoneme
*/
bool LabelMaker::moveToPrevPhoneme(ConstNoteItr& nItr, ConstSyllableItr& sItr, ConstPhonemeItr& pItr, bool skipRests) const
{
   if ((*sItr)->childBegin() == pItr) {
      if (false == moveToPrevSyllable(nItr, sItr, skipRests)) {
         return false;
      }
      pItr = (*sItr)->childEnd() - 1;
   } else {
      --pItr;
   }
   return true;
}

/*!
 @internal

 move to next phoneme
*/
bool LabelMaker::moveToNextPhoneme(ConstNoteItr& nItr, ConstSyllableItr& sItr, ConstPhonemeItr& pItr, bool skipRests) const
{
   if ((*sItr)->childEnd() - 1 == pItr) {
      if (false == moveToNextSyllable(nItr, sItr, skipRests)) {
         return false;
      }
      pItr = (*sItr)->childBegin();
   } else {
      ++pItr;
   }
   return true;
}

/*!
 @internal

 set label data
*/
void LabelMaker::setLabelData(LabelData& label, const ConstNoteItr& noteItr, const ConstSyllableItr& syllableItr, const ConstPhonemeItr& phonemeItr, int overwriteEnableFlag, int timeFlag) const
{
   {
      size_t j1(syllableNum / measureList.size());
      label.set('j', 1, std::min<size_t>(j1, 99));
   }
   {
      size_t j2(noteList.size() / measureList.size());
      label.set('j', 2, std::min<size_t>(j2, 99));
   }
   label.set('j', 3, phraseList.size());

   // Note
   NoteLabeler* note(*noteItr);
   NoteLabeler* pNote(NULL);
   NoteLabeler* nNote(NULL);
   {
      ConstNoteItr nItr(noteItr);
      if (moveToPrevNote(nItr, true)) {
         pNote = *nItr;
      }
   }
   {
      ConstNoteItr nItr(noteItr);
      if (moveToNextNote(nItr, true)) {
         nNote = *nItr;
      }
   }

   // Syllable
   SyllableLabeler* syllable(*syllableItr);
   SyllableLabeler* pSyllable(NULL);
   SyllableLabeler* nSyllable(NULL);
   {
      ConstNoteItr nItr(noteItr);
      ConstSyllableItr sItr(syllableItr);
      if (moveToPrevSyllable(nItr, sItr, true)) {
         pSyllable = *sItr;
      }
   }
   {
      ConstNoteItr nItr(noteItr);
      ConstSyllableItr sItr(syllableItr);
      if (moveToNextSyllable(nItr, sItr, true)) {
         nSyllable = *sItr;
      }
   }

   // Phoneme
   PhonemeLabeler* phoneme(*phonemeItr);
   PhonemeLabeler* pPhoneme(NULL);
   PhonemeLabeler* nPhoneme(NULL);
   PhonemeLabeler* ppPhoneme(NULL);
   PhonemeLabeler* nnPhoneme(NULL);
   {
      ConstNoteItr nItr(noteItr);
      ConstSyllableItr sItr(syllableItr);
      ConstPhonemeItr pItr(phonemeItr);
      if (moveToPrevPhoneme(nItr, sItr, pItr, false)) {
         pPhoneme = *pItr;
         if (moveToPrevPhoneme(nItr, sItr, pItr, false)) {
            ppPhoneme = *pItr;
         }
      }
   }
   {
      ConstNoteItr nItr(noteItr);
      ConstSyllableItr sItr(syllableItr);
      ConstPhonemeItr pItr(phonemeItr);
      if (moveToNextPhoneme(nItr, sItr, pItr, false)) {
         nPhoneme = *pItr;
         if (moveToNextPhoneme(nItr, sItr, pItr, false)) {
            nnPhoneme = *pItr;
         }
      }
   }

   // Note
   {
      NoteLabel l(label);
      note->setLabel(l);
   }
   if (pNote) {
      PrevNoteLabel l(label);
      pNote->setLabel(l);
   }
   if (nNote) {
      NextNoteLabel l(label);
      nNote->setLabel(l);
   }

   // Syllable
   {
      CurrentSyllableLabel l(label);
      syllable->setLabel(l);
   }
   if (pSyllable) {
      PrevSyllableLabel l(label);
      pSyllable->setLabel(l);
   }
   if (nSyllable) {
      NextSyllableLabel l(label);
      nSyllable->setLabel(l);
   }

   // Phoneme
   {
      CurrentPhonemeLabel l(label);
      phoneme->setLabel(l, overwriteEnableFlag);
   }
   if (ppPhoneme) {
      PrevPrevPhonemeLabel l(label);
      ppPhoneme->setLabel(l, overwriteEnableFlag);
   }
   if (pPhoneme) {
      PrevPhonemeLabel l(label);
      pPhoneme->setLabel(l, overwriteEnableFlag);
   }
   if (nPhoneme) {
      NextPhonemeLabel l(label);
      nPhoneme->setLabel(l, overwriteEnableFlag);
   }
   if (nnPhoneme) {
      NextNextPhonemeLabel l(label);
      nnPhoneme->setLabel(l, overwriteEnableFlag);
   }
}

/*!
 @internal

 apply stocks of score data
*/
void LabelMaker::applyStocks()
{
   // add measures from stocks to measure list
   while (!stockMeasures.empty()) {
      if (!measureList.empty()) {
         if (separateWholeNoteRests && !noteList.empty() && noteList.back()->isRest()
             && (0 < measureList.back()->getPosition().getCount()) && (1 <= stockMeasures.size())) { // divide rest
            // separate whole note rests
            NoteLabeler* lastNote(noteList.back());

            INT64 dur(lastNote->getMeasureIdx().getDuration()); // start point of the last note
            INT64 maxDur(measureList.back()->getDuration()); // total duration of this measure

            INT64 d(maxDur - dur); // residual duration of this measure
            if ((d < 0) || (std::numeric_limits<size_t>::max() < d)) {
               throw std::runtime_error("LabelMaker::applyStocks() duration has invalud value");
            }

            size_t newDur = 0;
            LabelMeasure* targetMeasure = NULL;
            INT64 nextMeasureDur = stockMeasures.front()->getDuration();

            if (maxDur == d) { // start from head of this measure
               if (nextMeasureDur + d <= lastNote->getDuration()) { // tail of this note is equal to or over tail of next measure
                  // divide note at tail of this measure
                  newDur = d;
                  targetMeasure = stockMeasures.front();
               }
            } else { // start from middle of this measure
               if (nextMeasureDur + d < lastNote->getDuration()) { // tail of this note is over tail of next measure
                  INT64 nextNextMeasureDur = stockMeasures[1]->getDuration();
                  if (nextMeasureDur + nextNextMeasureDur + d <= lastNote->getDuration()) { // tail of this note is equal to or over tail of next next measure
                     // divide note at tail of next measure
                     newDur = d + nextMeasureDur;
                     targetMeasure = stockMeasures[1]; // second measure
                  }
               }
            }

            if (0 < newDur) {
               NoteLabeler* newNote(new NoteLabeler(lastNote->getBeat(), lastNote->getDynamics(), lastNote->getKey()));
               newNote->setMeasure(targetMeasure);
               lastNote->moveTo(*newNote, static_cast<size_t>(newDur));
               noteList.push_back(newNote);
               if (inCrescendo) {
                  newNote->setCrescendo(crescendoList.back());
               }
               if (inDiminuendo) {
                  newNote->setDiminuendo(diminuendoList.back());
               }
            } else {
               LabelPosition pos(measureList.back()->getPosition());
               LabelPosition maxPos(measureList.back()->getMaxPosition());
               if (maxPos.getTime() < pos.getTime()) {
                  LabelPosition diff(pos - maxPos);
                  diff.setCount(0);
                  stockMeasures.front()->addPosition(diff);
               }
            }

         } else {
            LabelPosition pos(measureList.back()->getPosition());
            LabelPosition maxPos(measureList.back()->getMaxPosition());
            if (maxPos.getTime() < pos.getTime()) {
               LabelPosition diff(pos - maxPos);
               diff.setCount(0);
               stockMeasures.front()->addPosition(diff);
            }
         }
      }
      measureList.push_back(stockMeasures.front());
      stockMeasures.pop_front();
   }

   *this << tempScore;
   tempScore.clear();
}

};  // namespace sinsy

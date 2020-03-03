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

#ifndef SINSY_NOTE_LABELER_H_
#define SINSY_NOTE_LABELER_H_

#include <vector>
#include "Pitch.h"
#include "Beat.h"
#include "Key.h"
#include "Syllabic.h"
#include "Dynamics.h"
#include "Note.h"
#include "IConvertable.h"
#include "LabelPosition.h"
#include "SyllableLabeler.h"
#include "NoteGroup.h"
#include "LabelMeasure.h"
#include "INoteLabel.h"
#include "PhonemeInfo.h"
#include "util_converter.h"

namespace sinsy
{

class NoteLabeler : public IConvertable
{
public:
   typedef std::vector<SyllableLabeler*> List;

   //! constructor
   NoteLabeler(const Beat& b, const Dynamics& d, const Key& k);

   //! destructor
   virtual ~NoteLabeler();

   //! is rest or not
   virtual bool isRest() const;

   //! already converted or not
   virtual bool isConverted() const;

   //! get lyric
   virtual std::string getLyric() const;

   //! get syllabic
   virtual Syllabic getSyllabic() const;

   //! get duration
   size_t getDuration() const;

   //! add information
   virtual void addInfo(const std::vector<PhonemeInfo>& phonemes, const std::string& language, const std::string& info);

   //! set label
   virtual void setLabel(INoteLabel& label) const;

   //! add note
   void addNote(const Note& note, double tempo);

   //! get beat
   const Beat& getBeat() const;

   //! get dynamics
   const Dynamics& getDynamics() const;

   //! get key
   const Key& getKey() const;

   //! set measure
   void setMeasure(LabelMeasure* m);

   //! set phrase
   void setPhrase(NoteGroup* ng);

   //! set crescendo
   void setCrescendo(NoteGroup* ng);

   //! set diminuendo
   void setDiminuendo(NoteGroup* ng);

   //! set prev phrase
   void setPrevPhrase(const NoteGroup* ng);

   //! set next phrase
   void setNextPhrase(const NoteGroup* ng);

   //! get phrase
   const NoteGroup* getPhrase() const;

   //! get pitch
   Pitch getPitch() const;

   //! set positions
   void setPositions();

   //! is slur start or not
   bool isSlurStart() const;

   //! is slur stop or not
   bool isSlurStop() const;

   //! set if this note is in slur from previous note or not
   void setInSlurFromPrev(bool b);

   //! set if this note is in slur to next note or not
   void setInSlurToNext(bool b);

   //! set previous pitch
   void setPrevPitch(const Pitch& p);

   //! set next pitch
   void setNextPitch(const Pitch& p);

   //! get length
   LabelPosition getLength() const;

   //! set next accent position
   void setNextAccentPosition(const LabelPosition& p);

   //! set previous accent position
   void setPrevAccentPosition(const LabelPosition& p);

   //! set next staccato position
   void setNextStaccatoPosition(const LabelPosition& p);

   //! set previous staccato position
   void setPrevStaccatoPosition(const LabelPosition& p);

   //! get number of syllables
   size_t getSyllableNum() const;

   //! has accent or not
   bool hasAccent() const;

   //! has staccato or not
   bool hasStaccato() const;

   //! if this note has breath between this and next node, return true.
   bool hasBreathToNext() const;

   //! set previous note (null -> this is the head note)
   void setPrevNote(const NoteLabeler* prev);

   //! set next note (null -> this is the tail note)
   void setNextNote(const NoteLabeler* next);

   //! ...
   const LabelPosition& getMeasureIdx() const;

   //! ...
   void moveTo(NoteLabeler&, size_t = 0);

   //! set breath phoneme to the tail of the last syllable
   void setBreathPhoneme();

   //! get begin iteration
   List::const_iterator childBegin() const;

   //! get end iteration
   List::const_iterator childEnd() const;

private:
   //! copy constructor (donot use)
   NoteLabeler(const NoteLabeler&);

   //! assignment operator (donot use)
   NoteLabeler& operator=(const NoteLabeler&);

   //! connect lyrics
   std::string connectLyrics();

   //! list of children
   List children;

   //! previous note
   const NoteLabeler* prevNote;

   //! next note
   const NoteLabeler* nextNote;

   class NoteData
   {
   public:
      //! constructor
      NoteData(const Note& n, double t);

      //! destructor
      virtual ~NoteData();

      //! get note
      const Note& getNote() const;

      //! get tempo
      double getTempo() const;

      //! get length
      LabelPosition getLength() const;

   private:
      //! copy constructor (donot use)
      NoteData(const NoteData&);

      //! assignment operator (donot use)
      NoteData& operator=(const NoteData&);

      //! note
      Note note;

      //! tempo
      double tempo;

      friend class NoteLabeler;
   };

   class PositionAdder
   {
   public:
      //! constructor
      explicit PositionAdder(LabelPosition& p);

      //! copy constructor
      PositionAdder(const PositionAdder& obj);

      //! destructor
      virtual ~PositionAdder();

      //! ...
      void operator()(NoteData* data);

   private:
      //! assignment operator (donot use)
      PositionAdder& operator=(const PositionAdder&);

      //! position
      LabelPosition& position;
   };

   //! lyric
   std::string lyric;

   //! beat
   const Beat beat;

   //! dynamics
   const Dynamics dynamics;

   //! key
   const Key key;

   //! in slur from previous note or not
   bool inSlurFromPrev;

   //! in slur to next note or not
   bool inSlurToNext;

   //! pitch difference from previous note
   int pitchDifferenceFromPrev;

   //! pitch difference to next note
   int pitchDifferenceToNext;

   //! has prevois pitch or not
   bool hasPrevPitch;

   //! has next pitch or not
   bool hasNextPitch;

   //! previous accent
   LabelPosition prevAccent;

   //! next accent
   LabelPosition nextAccent;

   //! previous staccato
   LabelPosition prevStaccato;

   //! next staccato
   LabelPosition nextStaccato;

   //! measure
   LabelMeasure* measure;

   //! phrase
   NoteGroup* phrase;

   //! crescendo
   const NoteGroup* crescendo;

   //! diminuendo
   const NoteGroup* diminuendo;

   //! previous phrase
   const NoteGroup* prevPhrase;

   //! next phrase
   const NoteGroup* nextPhrase;

   //! meausre index
   LabelPosition measureIdx;

   //! phrase index
   LabelPosition phraseIdx;

   //! crescendo index
   LabelPosition crescendoIdx;

   //! diminuendo index
   LabelPosition diminuendoIdx;

   typedef std::vector<NoteData*> DataList;

   //! data list
   DataList dataList;
};

};

#endif // SINSY_NOTE_LABELER_H_

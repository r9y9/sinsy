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

#ifndef SINSY_TEMP_SCORE_H_
#define SINSY_TEMP_SCORE_H_

#include <vector>
#include "util_string.h"
#include "IScoreWritable.h"
#include "IScoreWriter.h"
#include "Note.h"
#include "Beat.h"
#include "Dynamics.h"
#include "Key.h"

namespace sinsy
{

class TempScore : public IScoreWritable, public IScoreWriter
{
public:
   class ITempData : public IScoreWriter
   {
   public:
      //! destructor
      virtual ~ITempData() {}

      //! write
      virtual void write(IScoreWritable&) const = 0;
   };

   //! constructor
   TempScore();

   //! destructor
   virtual ~TempScore();

   //! clear
   virtual void clear();

   //! write
   virtual void write(IScoreWritable&) const;

   //! set encoding
   virtual void setEncoding(const std::string& e);

   //! change tempo
   virtual void changeTempo(double);

   //! change beat
   virtual void changeBeat(const Beat&);

   //! change dynamics
   virtual void changeDynamics(const Dynamics&);

   //! change key
   virtual void changeKey(const Key&);

   //! start crescendo
   virtual void startCrescendo();

   //! start diminuendo
   virtual void startDiminuendo();

   //! stop crescendo
   virtual void stopCrescendo();

   //! stop diminuendo
   virtual void stopDiminuendo();

   //! add note
   virtual void addNote(const Note&);

protected:
   typedef std::vector<ITempData*> TempList;

   //! list of temporaries
   TempList tempList;

   class EncodingSetter : public ITempData
   {
   public:
      //! constructor
      explicit EncodingSetter(const std::string& e);

      //! destructor
      virtual ~EncodingSetter();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      EncodingSetter(const EncodingSetter&);

      //! assignment operator (donot use)
      EncodingSetter& operator=(const EncodingSetter&);

      //! encoding
      const std::string encoding;
   };

   class NoteAdder : public ITempData
   {
   public:
      //! constructor
      explicit NoteAdder(const Note& n);

      //! destructor
      virtual ~NoteAdder();

      //! wtrite
      virtual void write(IScoreWritable& sm) const;

      //! get note
      const Note& getNote() const;

      //! get note
      Note& getNote();

   private:
      //! copy constructor (donot use)
      NoteAdder(const NoteAdder&);

      //! assignment operator (donot use)
      NoteAdder& operator=(const NoteAdder&);

      //! note
      Note note;
   };

   class TempoChanger : public ITempData
   {
   public:
      //! constructor
      explicit TempoChanger(double t);

      //! destructor
      virtual ~TempoChanger();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      TempoChanger(const TempoChanger&);

      //! assignment operator (donot use)
      TempoChanger& operator=(const TempoChanger&);

      //! tempo
      double tempo;
   };

   class BeatChanger : public ITempData
   {
   public:
      //! constructor
      explicit BeatChanger(const Beat& b);

      //! destructor
      virtual ~BeatChanger();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      BeatChanger(const BeatChanger&);

      //! assignment operator (donot use)
      BeatChanger& operator=(const BeatChanger&);

      //! beat
      Beat beat;
   };

   class DynamicsChanger : public ITempData
   {
   public:
      //! constructor
      explicit DynamicsChanger(const Dynamics& d);

      //! destructor
      virtual ~DynamicsChanger();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      DynamicsChanger(const DynamicsChanger&);

      //! assignment operator (donot use)
      DynamicsChanger& operator=(const DynamicsChanger&);

      //! dynamics
      Dynamics dynamics;
   };

   class KeyChanger : public ITempData
   {
   public:
      //! constructor
      explicit KeyChanger(const Key& k);

      //! destructor
      virtual ~KeyChanger();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      KeyChanger(const KeyChanger&);

      //! assignment operator (donot use)
      KeyChanger& operator=(const KeyChanger&);

      //! key
      Key key;
   };

   class CrescendoStarter : public ITempData
   {
   public:
      //! constructor
      CrescendoStarter();

      //! destructor
      virtual ~CrescendoStarter();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      CrescendoStarter(const CrescendoStarter&);

      //! assignment operator (donot use)
      CrescendoStarter& operator=(const CrescendoStarter&);
   };

   class CrescendoStopper : public ITempData
   {
   public:
      //! constructor
      CrescendoStopper();

      //! destructor
      virtual ~CrescendoStopper();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      CrescendoStopper(const CrescendoStopper&);

      //! assignment operator (donot use)
      CrescendoStopper& operator=(const CrescendoStopper&);
   };

   class DiminuendoStarter : public ITempData
   {
   public:
      //! constructor
      DiminuendoStarter();

      //! destructor
      virtual ~DiminuendoStarter();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      DiminuendoStarter(const DiminuendoStarter&);

      //! assignment operator (donot use)
      DiminuendoStarter& operator=(const DiminuendoStarter&);
   };

   class DiminuendoStopper : public ITempData
   {
   public:
      //! constructor
      DiminuendoStopper();

      //! destructor
      virtual ~DiminuendoStopper();

      //! write
      virtual void write(IScoreWritable& sm) const;

   private:
      //! copy constructor (donot use)
      DiminuendoStopper(const DiminuendoStopper&);

      //! assignment operator (donot use)
      DiminuendoStopper& operator=(const DiminuendoStopper&);
   };


private:
   //! copy constructor (donot use)
   TempScore(const TempScore&);

   //! assignment operator (donot use)
   TempScore& operator=(const TempScore&);


   class DataSetter
   {
   public:
      //! constructor
      explicit DataSetter(IScoreWritable& sm);

      //! destructor
      virtual ~DataSetter();

      //! ...
      void operator()(const ITempData* data);

   private:
      //! maker
      IScoreWritable& maker;
   };
};

};

#endif // SINSY_TEMP_SCORE_H_

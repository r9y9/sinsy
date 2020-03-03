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

#ifndef SINSY_XML_WRITER_H_
#define SINSY_XML_WRITER_H_

#include "ForEachAdapter.h"
#include "Beat.h"
#include "TempScore.h"
#include "XmlData.h"
#include "IScoreWritable.h"

namespace sinsy
{

class WritableStrStream;
class Converter;

class XmlWriter : public IScoreWritable
{
public:
   typedef size_t Clef;
   static const Clef CLEF_DEFAULT;
   static const Clef CLEF_G;
   static const Clef CLEF_F;
   static const Clef CLEF_C;

public:
   //! constructor
   XmlWriter();

   //! destructor
   virtual ~XmlWriter();

   //! clear;
   void clear();

   //! get xml data
   const XmlData* getXmlData() const;

   // set encoding
   virtual void setEncoding(const std::string& encoding);

   //! change tempo
   virtual void changeTempo(double tempo);

   //! change beat
   virtual void changeBeat(const Beat& beat);

   //! change dynamics
   virtual void changeDynamics(const Dynamics& dynamics);

   //! change key
   virtual void changeKey(const Key& key);

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

   //! write xml to stream
   bool writeXml(WritableStrStream& stream) const;

   //! set clef
   void setClef(Clef clef);


private:
   //! copy constructor (donot use)
   XmlWriter(const XmlWriter&);

   //! assignment operator (donot use)
   XmlWriter& operator=(const XmlWriter&);

   //! set note tag
   void setNoteTag(const Note& note);

   //! set wedge tag
   void setWedgeTag(const std::string& type);

   //! set tags of head measure
   void setHeadMeasureTag();

   //! initialize xml data
   void initXmlData();

   //! fix measure
   void fixMeasure();

   //! get last measure
   XmlData* getLastMeasure();

   //! add xml data
   void addXmlData(XmlData* att);

   //! xml data
   XmlData* xmlData;

   //! encoding
   std::string encoding;

   //! part tag
   XmlData* part;

   //! last beat
   Beat lastBeat;

   //! last key
   Key lastKey;

   //! last measure
   XmlData* lastMeasure;

   //! number of last measure
   size_t lastMeasureNumber;

   //! last syllabic
   Syllabic lastSyllabic;

   //! duration to check end of measure
   size_t duration;

   Clef clef;
};

};

#endif // SINSY_XML_CONVERTER_H_

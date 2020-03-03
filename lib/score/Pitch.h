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

#ifndef SINSY_PITCH_H_
#define SINSY_PITCH_H_

#include <string>
#include <iostream>
#include "IStringable.h"

namespace sinsy
{

class Pitch
{
public:
   typedef int Step;
   static const Step C;
   static const Step Db;
   static const Step D;
   static const Step Eb;
   static const Step E;
   static const Step F;
   static const Step Gb;
   static const Step G;
   static const Step Ab;
   static const Step A;
   static const Step Bb;
   static const Step B;

   typedef int Octave;
   static const Step DEFAULT_STEP;
   static const Octave DEFAULT_OCTAVE;

   //! constructor
   Pitch();

   //! constructor
   Pitch(Step s, Octave o, int a = 0);

   //! constructor
   Pitch(const std::string& s, Octave o, int a = 0);

   //! copy constructor
   Pitch(const Pitch& obj);

   //! destructor
   virtual ~Pitch();

   //! assignment operator
   Pitch& operator=(const Pitch& obj);

   //! equal
   bool operator==(const Pitch& obj) const;

   //! not equrl
   bool operator!=(const Pitch& obj) const;

   //! set
   void set(Step s, Octave o, int alter = 0);

   //! set
   void set(const std::string& s, Octave o, int alter = 0);

   //! get step
   Step getStep() const;

   //! get octave
   Octave getOctave() const;

   //! get string of step
   const std::string& getStepStr() const;

   //! addition
   Pitch& operator+=(int i);

   //! subtraction
   Pitch& operator-=(int i);

   //! increment
   Pitch& operator++();

   //! decrement
   Pitch& operator--();

private:
   //! pitch step
   Step step;

   //! pitch octave
   Octave octave;
};

//! addition
Pitch operator+(const Pitch& pitch, int i);

//! subtraction
Pitch operator-(const Pitch& pitch, int i);

//! increment
Pitch operator++(const Pitch& pitch);

//! decrement
Pitch operator--(const Pitch& pitch);

//! to stream
std::ostream& operator<<(std::ostream&, const Pitch&);
};

#endif // SINSY_PITCH_H_

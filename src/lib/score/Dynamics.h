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

#ifndef SINSY_DYNAMICS_H_
#define SINSY_DYNAMICS_H_

#include <string>

namespace sinsy
{

class Dynamics
{
public:
   static const Dynamics PPPP;
   static const Dynamics PPP;
   static const Dynamics PP;
   static const Dynamics P;
   static const Dynamics MP;
   static const Dynamics N;
   static const Dynamics MF;
   static const Dynamics F;
   static const Dynamics FF;
   static const Dynamics FFF;
   static const Dynamics FFFF;

   //! constructor
   Dynamics();

   //! constructor
   explicit Dynamics(const std::string& str);

   //! copy constructor
   Dynamics(const Dynamics& obj);

   //! destructor
   virtual ~Dynamics();

   //! assignment operator
   Dynamics& operator=(const Dynamics& obj);

   //! equal
   bool operator==(const Dynamics& obj) const;

   //! not equal
   bool operator!=(const Dynamics& obj) const;

   //! set value
   void set(const std::string& str);

   //! get value as string
   const std::string& getStr() const;

   //! get tag string
   const std::string& getTagStr() const;

private:
   //! value
   size_t value;
};

//! to string
std::ostream& operator<<(std::ostream& os, const Dynamics& dynamics);
};

#endif // SINSY_DYNAMICS_H_

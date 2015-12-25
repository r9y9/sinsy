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

#ifndef SINSY_PHONEME_TABLE_H_
#define SINSY_PHONEME_TABLE_H_

#include <map>
#include <vector>
#include <string>

namespace sinsy
{

class PhonemeTable
{
public:
   typedef std::vector<std::string> PhonemeList;

   class Result
   {
   public:
      //! constructor
      Result();

      //! constructor
      Result(const std::string& s, const PhonemeList* p);

      //! copy constructor
      Result(const Result& obj);

      //! destructor
      virtual ~Result();

      //! assignment operator
      Result& operator=(const Result& obj);

      //! is valid or not
      bool isValid() const;

      //! get syllable
      const std::string& getSyllable() const;

      //! get phoneme list
      const PhonemeList* getPhonemeList() const;

      //! get mached length
      size_t getMatchedLength() const;

   private:
      //! syllable
      const std::string* syllable;

      //! phoneme list
      const PhonemeList* phonemeList;
   };

public:
   //! constructor
   PhonemeTable();

   //! destructor
   virtual ~PhonemeTable();

   //! clear
   void clear();

   //! read from file
   bool read(const std::string& fname);

   //! find from table
   Result find(const std::string& syllable) const;

   //! return matched result
   Result match(const std::string& syllable) const;

private:
   //! copy constructor (donot use)
   PhonemeTable(const PhonemeTable&);

   //! assignment operator (donot use)
   PhonemeTable& operator=(const PhonemeTable&);

   typedef std::map<std::string, PhonemeList*> ConvertTable;

   //! convert table
   ConvertTable convertTable;
};

};

#endif // SINSY_PHONEME_TABLE_H_

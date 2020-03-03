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

#include "InputFile.h"
#include "util_log.h"

namespace sinsy
{

/*!
 constructor
 */
InputFile::InputFile()
{
}

/*!
 constructor
 */
InputFile::InputFile(const std::string& fpath) : stream(fpath.c_str())
{
}

/*!
 destructor
 */
InputFile::~InputFile()
{
   stream.close();
}

/*!
 read data from stream

 @param buffer buffer for read data
 @param byte   byte you want to read
 @return       read bytes (0 : end of stream)
 */
size_t InputFile::read(void* buffer, size_t size) throw (StreamException)
{
   if(stream.fail()) {
      throw StreamException("InputFile::read()");
   }
   if (stream.eof()) {
      return 0;
   }
   stream.read(static_cast<char*>(buffer), size);
   int ret = stream.gcount();
   if (ret <= 0) {
      ERR_MSG("File reading error (size: " << size << ")  result of gcount = " << ret);
      throw StreamException("InputFile::read()");
   }
   return ret;
}

/*!
 open
 */
void InputFile::open(const std::string& fpath)
{
   stream.open(fpath.c_str());
}

/*!
 close
 */
void InputFile::close()
{
   stream.close();
}

/*!
 stream is valid or not
 */
bool InputFile::isValid() const
{
   return !stream.fail();
}

/*!
 read a line
 */
bool InputFile::readLine(std::string& line)
{
   if (!isValid() || isEof()) {
      return false;
   }
   getline(stream, line);
   return true;
}

};  // namespace sinsy

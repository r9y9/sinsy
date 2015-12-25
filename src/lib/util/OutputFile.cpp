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

#include "util_log.h"
#include "OutputFile.h"

namespace sinsy
{

/*!
 constructor
 */
OutputFile::OutputFile()
{
}

/*!
 constructor
 */
OutputFile::OutputFile(const std::string& fpath) : stream(fpath.c_str())
{
}

/*!
 destructor
 */
OutputFile::~OutputFile()
{
   stream.close();
}

/*!
 write data to stream
 @param buffer buffer for data that you want to write
 @param byte   byte you want to write
 @return       write bytes
 */
size_t OutputFile::write(const void* buffer, size_t size) throw (StreamException)
{
   if (stream.fail()) {
      throw StreamException("OutputFile::write()");
   }
   stream.write(static_cast<const char*>(buffer), size);
   if (stream.fail()) {
      ERR_MSG("File writing error (size: " << size << ")");
      throw StreamException("OutputFile::write()");
   }
   return size;
}

/*!
 open
 */
void OutputFile::open(const std::string& fpath)
{
   stream.open(fpath.c_str());
}

/*!
 close
 */
void OutputFile::close()
{
   stream.close();
}

/*!
 stream is valid or not
 */
bool OutputFile::isValid() const
{
   return !stream.fail();
}

};  // namespace sinsy

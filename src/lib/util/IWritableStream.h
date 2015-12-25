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

#ifndef SINSY_I_WRITE_STREAM_H_
#define SINSY_I_WRITE_STREAM_H_

#include "util_types.h"
#include "StreamException.h"

namespace sinsy
{

class IWritableStream
{
public:
   //! destructor
   virtual ~IWritableStream() {}

   /*!
    write data to stream
    @param buffer buffer for data that you want to write
    @param byte   byte you want to write
    @return       write bytes
   */
   virtual size_t write(const void* buffer, size_t byte) throw (StreamException) = 0;
};

/*!
 write data to stream
 */
template<class T>
IWritableStream& toStream(IWritableStream& stream, const T& value)
{
   size_t idx(0);
   const size_t size(sizeof(value));
   while (idx < size) {
      size_t result(stream.write(reinterpret_cast<const char*>(&value) + idx, size - idx));
      if (result <= 0) {
         throw StreamException("cannot write to IWritableStream");
      }
      idx += result;
   }
   return stream;
}

/*!
 write data to stream (char)
 */
inline IWritableStream& operator<<(IWritableStream& stream, char buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (unsigned char)
 */
inline IWritableStream& operator<<(IWritableStream& stream, unsigned char buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (INT16)
 */
inline IWritableStream& operator<<(IWritableStream& stream, INT16 buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (UINT16)
 */
inline IWritableStream& operator<<(IWritableStream& stream, UINT16 buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (INT32)
 */
inline IWritableStream& operator<<(IWritableStream& stream, INT32 buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (UINT32)
 */
inline IWritableStream& operator<<(IWritableStream& stream, UINT32 buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (INT64)
 */
inline IWritableStream& operator<<(IWritableStream& stream, INT64 buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (UINT64)
 */
inline IWritableStream& operator<<(IWritableStream& stream, UINT64 buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (float)
 */
inline IWritableStream& operator<<(IWritableStream& stream, float buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (double)
 */
inline IWritableStream& operator<<(IWritableStream& stream, double buf) throw (StreamException)
{
   return toStream(stream, buf);
}

/*!
 write data to stream (long double)
 */
inline IWritableStream& operator<<(IWritableStream& stream, long double buf) throw (StreamException)
{
   return toStream(stream, buf);
}

};

#endif // SINSY_I_WRITE_STREAM_H_

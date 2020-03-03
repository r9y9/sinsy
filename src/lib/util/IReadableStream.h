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

#ifndef SINSY_I_READ_STREAM_H_
#define SINSY_I_READ_STREAM_H_

#include "util_types.h"
#include "StreamException.h"

namespace sinsy
{

class IReadableStream
{
public:
   //! destructor
   virtual ~IReadableStream() {}

   /*!
    read data from stream

    @param buffer buffer for read data
    @param byte   byte you want to read
    @return       read bytes (0 : end of stream)
   */
   virtual size_t read(void* buffer, size_t byte) throw (StreamException) = 0;
};

/*!
 read data from stream
 */
template<class T>
IReadableStream& fromStream(IReadableStream& stream, T& buf) throw (StreamException)
{
   size_t idx(0);
   const size_t sz(sizeof(buf));
   while (idx < sz) {
      size_t result(stream.read(reinterpret_cast<char*>(&buf) + idx, sz - idx));
      if (result <= 0) {
         throw StreamException("cannot read from IReadableStream");
      }
      idx += result;
   }
   return stream;
}

/*!
 read data from stream (char)
 */
inline IReadableStream& operator>>(IReadableStream& stream, char& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (unsigned char)
 */
inline IReadableStream& operator>>(IReadableStream& stream, unsigned char& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (INT16)
 */
inline IReadableStream& operator>>(IReadableStream& stream, INT16& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (UINT16)
 */
inline IReadableStream& operator>>(IReadableStream& stream, UINT16& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (INT32)
 */
inline IReadableStream& operator>>(IReadableStream& stream, INT32& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (UINT32)
 */
inline IReadableStream& operator>>(IReadableStream& stream, UINT32& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (INT64)
 */
inline IReadableStream& operator>>(IReadableStream& stream, INT64& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (UINT64)
 */
inline IReadableStream& operator>>(IReadableStream& stream, UINT64& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (float)
 */
inline IReadableStream& operator>>(IReadableStream& stream, float& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (double)
 */
inline IReadableStream& operator>>(IReadableStream& stream, double& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

/*!
 read data from stream (long double)
 */
inline IReadableStream& operator>>(IReadableStream& stream, long double& buf) throw (StreamException)
{
   return fromStream(stream, buf);
}

};

#endif // SINSY_I_READ_STREAM_H_

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

#ifndef SINSY_XML_TAGS_H_
#define SINSY_XML_TAGS_H_

#include "util_types.h"

namespace sinsy
{
const std::string TAG_ACCENT         = "accent";
const std::string TAG_ARTICULATIONS  = "articulations";
const std::string TAG_ALTER          = "alter";
const std::string TAG_ATTRIBUTES     = "attributes";
const std::string TAG_BEAT_TYPE      = "beat-type";
const std::string TAG_BEAT_UNIT      = "beat-unit";
const std::string TAG_BEATS          = "beats";
const std::string TAG_BREATH_MARK    = "breath-mark";
const std::string TAG_CLEF           = "clef";
const std::string TAG_DIVISIONS      = "divisions";
const std::string TAG_DIRECTION      = "direction";
const std::string TAG_DIRECTION_TYPE = "direction-type";
const std::string TAG_DURATION       = "duration";
const std::string TAG_DYNAMICS       = "dynamics";
const std::string TAG_ENCODING       = "encoding";
const std::string TAG_FIFTHS         = "fifths";
const std::string TAG_IDENTIFICATION = "identification";
const std::string TAG_KEY            = "key";
const std::string TAG_LINE           = "line";
const std::string TAG_LYRIC          = "lyric";
const std::string TAG_MEASURE        = "measure";
const std::string TAG_METRONOME      = "metronome";
const std::string TAG_MODE           = "mode";
const std::string TAG_NOTATIONS      = "notations";
const std::string TAG_NOTE           = "note";
const std::string TAG_OCTAVE         = "octave";
const std::string TAG_PART           = "part";
const std::string TAG_PER_MINUTE     = "per-minute";
const std::string TAG_PITCH          = "pitch";
const std::string TAG_REST           = "rest";
const std::string TAG_SCORE_PARTWISE = "score-partwise";
const std::string TAG_SIGN           = "sign";
const std::string TAG_SLUR           = "slur";
const std::string TAG_SOUND          = "sound";
const std::string TAG_STACCATO       = "staccato";
const std::string TAG_STEP           = "step";
const std::string TAG_SUPPORTS       = "supports";
const std::string TAG_SYLLABIC       = "syllabic";
const std::string TAG_TECHNICAL      = "technical";
const std::string TAG_TIE            = "tie";
const std::string TAG_TIED           = "tied";
const std::string TAG_TEXT           = "text";
const std::string TAG_TIME           = "time";
const std::string TAG_UP_BOW         = "up-bow";
const std::string TAG_WEDGE          = "wedge";
};

#endif // SINSY_XML_TAGS_H_

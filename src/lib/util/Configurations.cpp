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

#include <fstream>
#include "Configurations.h"
#include "util_log.h"
#include "util_string.h"
#include "StringTokenizer.h"

namespace sinsy
{

namespace
{
const std::string SEPARATOR = "=";
};

/*!
 constructor
 */
Configurations::Configurations()
{
}

/*!
 destructor
 */
Configurations::~Configurations()
{
   clear();
}

/*!
 clear
 */
void Configurations::clear()
{
   configs.clear();
}

/*!
 read configurations from file
 */
bool Configurations::read(const std::string& fpath)
{
   std::ifstream ifs(fpath.c_str());
   if (!ifs) {
      ERR_MSG("Cannot open config file : " << fpath);
      return false;
   }

   clear();

   std::string buffer;

   while (!ifs.eof()) {
      std::getline(ifs, buffer);

      // remove comments
      size_t idx = buffer.find("#");
      if (idx != std::string::npos) {
         buffer.resize(idx);
      }
      StringTokenizer st(buffer, SEPARATOR);

      size_t sz(st.size());
      if (0 == sz) {
         continue;
      } else if (2 != st.size()) {
         ERR_MSG("Syntax error : " << fpath);
         return false;
      }

      std::string key(st.at(0));
      cutBlanks(key);
      std::string value(st.at(1));
      cutBlanks(value);
      if (key.empty() || value.empty()) {
         ERR_MSG("Syntax error : " << fpath);
         return false;
      }

      // cut " and '
      if (1 < value.size()) {
         if ((('\"' == value[0]) && ('\"' == value[value.size() - 1])) || (('\'' == value[0]) && ('\'' == value[value.size() - 1]))) {
            value = value.substr(1, value.size() - 2);
            cutBlanks(value);
            if (value.empty()) {
               ERR_MSG("Syntax error : " << fpath);
               return false;
            }
         }
      }
      configs.insert(std::make_pair(key, value));
   }
   return true;
}

/*!
 for std::string (need default)
 */
template<>
std::string Configurations::get<std::string>(const std::string& key, const std::string& def) const
{
   Configs::const_iterator itr(configs.find(key));
   if (configs.end() != itr) {
      return itr->second;
   }
   return def;
}

/*!
 for std::string (not need default)
 */
std::string Configurations::get(const std::string& key) const
{
   Configs::const_iterator itr(configs.find(key));
   if (configs.end() != itr) {
      return itr->second;
   }
   return std::string();
}

/*!
 for bool
 */
template<>
bool Configurations::get<bool>(const std::string& key, const bool& def) const
{
   Configs::const_iterator itr(configs.find(key));
   if (configs.end() != itr) {
      std::string value(itr->second);
      toLower(value);

      if (value == "true") {
         return true;
      } else if (value == "false") {
         return false;
      }

      int i(-1);
      std::istringstream iss(value);
      iss >> i;
      if (0 == i) {
         return false;
      } else if (0 < i) {
         return true;
      }
   }
   return def;
}

};  // namespace sinsy

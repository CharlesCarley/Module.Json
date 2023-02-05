/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#pragma once

#include "Utils/Definitions.h"
#include "Utils/String.h"
#include "Json/Token.h"

namespace Rt2::Json
{

    class Scanner
    {
    private:
        char*  _data;
        size_t _len;
        size_t _pos;

        static bool isDigitSet(char ch);

    public:
        Scanner();
        ~Scanner();

        /// <summary>
        ///
        /// </summary>
        /// <param name="path">const String&</param>
        void open(const String& path);
        /// <summary>
        ///
        /// </summary>
        /// <param name="mem">const char*</param>
        /// <param name="len">size_t</param>
        void open(const char* mem, size_t len);

        /// <summary>
        ///
        /// </summary>
        /// <param name="tok">skJsonToken&</param>
        void scan(Token& tok);

        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        bool isOpen() const
        {
            return _data != nullptr && _pos != Npos;
        }
    };
}  // namespace Rt2::Json

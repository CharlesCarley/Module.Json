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

#include "Utils/skString.h"
#include "skJsonToken.h"

class skJsonScanner
{
private:
    SKint8* m_data;
    SKsize  m_len;
    SKsize  m_pos;

    static bool isDigitSet(char ch);

public:
    skJsonScanner();
    ~skJsonScanner();

    /// <summary>
    ///
    /// </summary>
    /// <param name="path">const skString&</param>
    void open(const skString& path);
    /// <summary>
    ///
    /// </summary>
    /// <param name="mem">const char*</param>
    /// <param name="len">SKsize</param>
    void open(const char* mem, SKsize len);

    /// <summary>
    ///
    /// </summary>
    /// <param name="tok">skJsonToken&</param>
    void scan(skJsonToken& tok);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    bool isOpen() const
    {
        return m_data != nullptr && m_pos != SK_NPOS;
    }
};

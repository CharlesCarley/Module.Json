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

class skJsonType;
class skJsonArray;
class skJsonObject;
class skJsonString;
class skJsonBoolean;
class skJsonInteger;

/// \ingroup Json
enum class skJsonTokenType
{
    JT_UNDEFINED,
    JT_COLON,
    JT_COMMA,
    JT_L_BRACE,
    JT_L_BRACKET,
    JT_R_BRACE,
    JT_R_BRACKET,
    JT_NUMBER,
    JT_INTEGER,
    JT_BOOL,
    JT_STRING,
    JT_NULL,
};

/// \ingroup Json
///
class skJsonToken
{
private:
    skString        m_value;
    skJsonTokenType m_type;

public:
    skJsonToken();
    ~skJsonToken() = default;

    /// <summary>
    ///
    /// </summary>
    /// <param name="value">char</param>
    void push(char value);
    /// <summary>
    ///
    /// </summary>
    /// <param name="value">const skString&</param>
    void push(const skString& value);

    /// <summary>
    ///
    /// </summary>
    void clear(void);

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skString& getValue(void) const
    {
        return m_value;
    }

    /// <summary>
    ///
    /// </summary>
    /// <returns></returns>
    const skJsonTokenType& getType(void) const
    {
        return m_type;
    }

    /// <summary>
    ///
    /// </summary>
    /// <param name="type">const skJsonTokenType&</param>
    void setType(const skJsonTokenType& type)
    {
        m_type = type;
    }
};

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
#ifndef _skJsonBoolean_h_
#define _skJsonBoolean_h_

#include "skJsonType.h"

/// \ingroup Json
class skJsonBoolean final : public skJsonType
{
private:
    bool m_bvalue;

public:
    skJsonBoolean() :
        skJsonType(Type::BOOLEAN),
        m_bvalue(false)
    {
    }

    void serialize(void)
    {
        skStringConverter::toString(m_value, m_bvalue);
    }
};

#endif  //_skJsonBoolean_h_

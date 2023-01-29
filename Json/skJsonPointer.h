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

#include "skJsonType.h"

class skJsonPointer final : public skJsonType
{
private:
    SKsize m_address;

    void notifyStringChanged() override
    {
        if (m_value.equals("null"))
            m_address = 0;
        else
            m_address = m_value.toUint64();
    }

    void notifyValueChanged() override
    {
        if (!m_address)
            m_value.assign("null");
        else
            skChar::toString(m_value, m_address);
    }

public:
    skJsonPointer() :
        skJsonType(Type::POINTER),
        m_address(0)
    {
    }

    skJsonPointer(const void* vp) :
        skJsonType(Type::POINTER),
        m_address((SKsize)vp)
    {
        notifyValueChanged();
    }

    void toString(skStringBuilder& dest) override
    {
        if (!m_address)
            dest.write("null");
        else
            dest.write(m_address);
    }
};

#endif  //_skJsonPointer_h_

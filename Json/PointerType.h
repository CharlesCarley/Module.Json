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

#include "Json/Type.h"


namespace Rt2::Json
{
    class PointerType final : public Type
    {
    private:
        size_t _address;

        void notifyStringChanged() override
        {
            if (_value == "null")
                _address = 0;
            else
                _address = Char::toUint64(_value);
        }

        void notifyValueChanged() override
        {
            if (!_address)
                _value.assign("null");
            else
                Char::toString(_value, _address);
        }

    public:
        PointerType() :
            Type(POINTER),
            _address(0)
        {
        }

        explicit PointerType(const void* vp) :
            Type(POINTER),
            _address((size_t)vp)
        {
            notifyValueChanged();
        }

        void toString(StringBuilder& dest) override
        {
            if (!_address)
                dest.write("null");
            else
                dest.write(_address);
        }
    };
}  // namespace Rt2::Json

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

#include "Type.h"

namespace Rt2::Json
{

    /// \ingroup Json
    class IntegerType final : public Type
    {
    private:
        typedef union Integer
        {
            I16 i16[4];
            I32 i32[2];
            I64 i64;
            U16 u16[4];
            U32 u32[2];
            U64 u64;
        } Integer;

        Integer _integer;

        void notifyStringChanged() override
        {
            _integer.i64 = Char::toInt64(_value);
        }

        void notifyValueChanged() override
        {
            Char::toString(_value, _integer.i64);
        }

    public:
        IntegerType() :
            Type(INTEGER),
            _integer({})
        {
        }

        explicit IntegerType(const I64& val) :
            Type(INTEGER),
            _integer({})
        {
            _integer.i64 = val;
            notifyValueChanged();
        }

        void toString(StringBuilder& dest) override
        {
            dest.write(_integer.i64);
        }
    };
}  // namespace Rt2::Json
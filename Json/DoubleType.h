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

    class DoubleType final : public Type
    {
    private:
        double _double;

        void notifyStringChanged() override
        {
            _double = Char::toDouble(_value);
        }

        void notifyValueChanged() override
        {
            Char::toString(_value, _double);
        }

    public:
        DoubleType() :
            Type(DOUBLE),
            _double(0.0)
        {
        }

        explicit DoubleType(const double& v) :
            Type(DOUBLE),
            _double(v)
        {
            notifyValueChanged();
        }

        void toString(StringBuilder& dest) override
        {
            dest.write(_double);
        }
    };
}  // namespace Rt2::Json

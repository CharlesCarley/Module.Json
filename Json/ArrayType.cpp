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
#include "ArrayType.h"
#include "BoolType.h"
#include "DoubleType.h"
#include "IntegerType.h"
#include "PointerType.h"
#include "StringType.h"

namespace Rt2::Json
{

    ArrayType::ArrayType() :
        Type(ARRAY)
    {
    }

    ArrayType::~ArrayType()
    {
        for (const auto& it : _array)
            delete it;
    }

    void ArrayType::add(Type* value)
    {
        if (value)
            _array.push_back(value);
    }

    void ArrayType::add(const I16& value)
    {
        add(new IntegerType(value));
    }

    void ArrayType::add(const I32& value)
    {
        add(new IntegerType(value));
    }

    void ArrayType::add(const I64& value)
    {
        add(new IntegerType(value));
    }

    void ArrayType::add(const U16& value)
    {
        add(new IntegerType(value));
    }

    void ArrayType::add(const U32& value)
    {
        add(new IntegerType(value));
    }

    void ArrayType::add(const U64& value)
    {
        add(new IntegerType(value));
    }

    void ArrayType::add(bool value)
    {
        add(new BoolType(value));
    }

    void ArrayType::add(const double& value)
    {
        add(new DoubleType(value));
    }

    void ArrayType::add(const float& value)
    {
        add(new DoubleType((double)value));
    }

    void ArrayType::add(const String& value)
    {
        add(new StringType(value));
    }

    void ArrayType::add(const void* value)
    {
        add(new PointerType(value));
    }

    void ArrayType::toString(StringBuilder& dest)
    {
        dest.write('[');

        for (U32 i = 0; i < _array.size(); ++i)
        {
            Type* type = _array.at(i);
            if (i > 0)
                dest.write(',');
            type->toString(dest);
        }

        dest.write(']');
    }
}  // namespace Rt2::Json

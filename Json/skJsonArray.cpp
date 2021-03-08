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
#include "skJsonArray.h"
#include "skJsonBoolean.h"
#include "skJsonDouble.h"
#include "skJsonInteger.h"
#include "skJsonPointer.h"
#include "skJsonString.h"

skJsonArray::skJsonArray() :
    skJsonType(Type::ARRAY)
{
}

skJsonArray::~skJsonArray()
{
    Array::Iterator it = m_array.iterator();
    while (it.hasMoreElements())
        delete it.getNext();
}

void skJsonArray::add(skJsonType* value)
{
    if (value)
        m_array.push_back(value);
}

void skJsonArray::add(const SKint16& value)
{
    add(new skJsonInteger(value));
}

void skJsonArray::add(const SKint32& value)
{
    add(new skJsonInteger(value));
}

void skJsonArray::add(const SKint64& value)
{
    add(new skJsonInteger(value));
}

void skJsonArray::add(const SKuint16& value)
{
    add(new skJsonInteger(value));
}

void skJsonArray::add(const SKuint32& value)
{
    add(new skJsonInteger(value));
}

void skJsonArray::add(const SKuint64& value)
{
    add(new skJsonInteger(value));
}

void skJsonArray::add(bool value)
{
    add(new skJsonBoolean(value));
}

void skJsonArray::add(const double& value)
{
    add(new skJsonDouble(value));
}

void skJsonArray::add(const float& value)
{
    add(new skJsonDouble((double)value));
}

void skJsonArray::add(const skString& value)
{
    add(new skJsonString(value));
}

void skJsonArray::add(const void* value)
{
    add(new skJsonPointer(value));
}

void skJsonArray::toString(skString& dest)
{
    dest.reserve(128);
    dest.resize(0);
    dest.append('[');

    skString tStr;
    for (SKuint32 i = 0; i < m_array.size(); ++i)
    {
        skJsonType* type = m_array.at(i);
        if (i > 0)
            dest.append(',');

        type->toString(tStr);
        dest.append(tStr);
    }
    dest.append(']');
}

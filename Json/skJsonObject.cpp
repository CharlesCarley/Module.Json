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
#include "skJsonObject.h"
#include "skJsonBoolean.h"
#include "skJsonDouble.h"
#include "skJsonInteger.h"
#include "skJsonPointer.h"

skJsonObject::skJsonObject() :
    skJsonType(Type::OBJECT)
{
}

skJsonObject::~skJsonObject()
{
    Dictionary::Iterator it = m_dictionary.iterator();
    while (it.hasMoreElements())
        delete it.getNext().second;

    m_dictionary.clear();
}

void skJsonObject::insert(const skString& key, skJsonType* value)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos == SK_NPOS)
        m_dictionary.insert(key, value);
}

bool skJsonObject::hasKey(const skString& key) const
{
    return m_dictionary.find(key) != SK_NPOS;
}

skJsonType* skJsonObject::getValue(const skString& key)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos != SK_NPOS)
        return m_dictionary.at(pos);
    return nullptr;
}

void skJsonObject::getValueString(skString& dest, const skString& key, const skString& def)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos != SK_NPOS)
        dest.assign(m_dictionary.at(pos)->getString());
    else
        dest.assign(def);
}

void skJsonObject::getValueInt(SKint64& dest, const skString& key, const SKint64& def)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos != SK_NPOS)
        dest = m_dictionary.at(pos)->getInt64(def);
    else
        dest = def;
}

void skJsonObject::getValueInt(SKint32& dest, const skString& key, const SKint32& def)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos != SK_NPOS)
        dest = m_dictionary.at(pos)->getInt32(def);
    else
        dest = def;
}

void skJsonObject::getValueInt(SKint16& dest, const skString& key, const SKint16& def)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos != SK_NPOS)
        dest = m_dictionary.at(pos)->getInt16(def);
    else
        dest = def;
}

bool skJsonObject::getBoolean(const skString& key, bool def)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos != SK_NPOS)
        return m_dictionary.at(pos)->getBoolean(def);
    return def;
}

double skJsonObject::getDouble(const skString& key, double def)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos != SK_NPOS)
        return m_dictionary.at(pos)->getDouble(def);
    return def;
}

float skJsonObject::getFloat(const skString& key, float def)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos != SK_NPOS)
        return (float)m_dictionary.at(pos)->getDouble((double)def);
    return def;
}

void skJsonObject::insert(const skString& key, const SKint16& value)
{
    insert(key, (SKint64)value);
}

void skJsonObject::insert(const skString& key, const SKint32& value)
{
    insert(key, (SKint64)value);
}

void skJsonObject::insert(const skString& key, const SKint64& value)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos == SK_NPOS)
        m_dictionary.insert(key, new skJsonInteger(value));
}

void skJsonObject::insert(const skString& key, const float& value)
{
    insert(key, (double)value);
}

void skJsonObject::insert(const skString& key, const double& value)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos == SK_NPOS)
        m_dictionary.insert(key, new skJsonDouble(value));
}

void skJsonObject::insert(const skString& key, const bool& value)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos == SK_NPOS)
        m_dictionary.insert(key, new skJsonBoolean(value));
}

void skJsonObject::insert(const skString& key, const void* value)
{
    const SKsize pos = m_dictionary.find(key);
    if (pos == SK_NPOS)
        m_dictionary.insert(key, new skJsonPointer(value));
}

void skJsonObject::toString(skString& dest)
{
    dest.reserve(128);
    dest.resize(0);
    dest.append('{');

    Dictionary::Iterator it = m_dictionary.iterator();
    while (it.hasMoreElements())
    {
        if (dest.at(dest.size() - 1) != '{')
            dest.append(',');

        const skString& key = it.peekNextKey();
        skJsonType*     val = it.peekNextValue();

        dest.append('"');
        dest.append(key);
        dest.append('"');
        dest.append(':');
        dest.append(val->toString());

        it.next();
    }

    dest.append('}');
}

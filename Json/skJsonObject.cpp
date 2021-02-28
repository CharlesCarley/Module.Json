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

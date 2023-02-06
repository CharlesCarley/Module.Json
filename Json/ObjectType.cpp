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
#include "ObjectType.h"
#include "BoolType.h"
#include "DoubleType.h"
#include "IntegerType.h"
#include "PointerType.h"

namespace Rt2::Json
{
    ObjectType::ObjectType() :
        Type(OBJECT)
    {
    }

    ObjectType::~ObjectType()
    {
        for (const auto& el : _dictionary)
            delete el.second;
        _dictionary.clear();
    }

    void ObjectType::insert(const String& key, Type* value)
    {
        if (const size_t pos = _dictionary.find(key); pos == Npos)
            _dictionary.insert(key, value);
    }

    bool ObjectType::hasKey(const String& key) const
    {
        return _dictionary.find(key) != Npos;
    }

    Type* ObjectType::find(const String& key)
    {
        if (const size_t pos = _dictionary.find(key);
            pos != Npos)
            return _dictionary.at(pos);
        return nullptr;
    }

    void ObjectType::string(String& dest, const String& key, const String& def)
    {
        if (const size_t pos = _dictionary.find(key);
            pos != Npos)
            dest.assign(_dictionary.at(pos)->string());
        else
            dest.assign(def);
    }

    void ObjectType::integer(I64& dest, const String& key, const I64& def)
    {
        if (const size_t pos = _dictionary.find(key);
            pos != Npos)
            dest = _dictionary.at(pos)->i64(def);
        else
            dest = def;
    }

    void ObjectType::integer(I32& dest, const String& key, const I32& def)
    {
        if (const size_t pos = _dictionary.find(key);
            pos != Npos)
            dest = _dictionary.at(pos)->i32(def);
        else
            dest = def;
    }

    void ObjectType::integer(I16& dest, const String& key, const I16& def)
    {
        if (const size_t pos = _dictionary.find(key);
            pos != Npos)
            dest = _dictionary.at(pos)->i16(def);
        else
            dest = def;
    }

    bool ObjectType::boolean(const String& key, const bool def)
    {
        if (const size_t pos = _dictionary.find(key);
            pos != Npos)
            return _dictionary.at(pos)->boolean(def);
        return def;
    }

    double ObjectType::r64(const String& key, const double def)
    {
        if (const size_t pos = _dictionary.find(key);
            pos != Npos)
            return _dictionary.at(pos)->r64(def);
        return def;
    }

    float ObjectType::r32(const String& key, const float def)
    {
        if (const size_t pos = _dictionary.find(key);
            pos != Npos)
            return (float)_dictionary.at(pos)->r64((double)def);
        return def;
    }

    void ObjectType::insert(const String& key, const I16& value)
    {
        insert(key, (I64)value);
    }

    void ObjectType::insert(const String& key, const I32& value)
    {
        insert(key, (I64)value);
    }

    void ObjectType::insert(const String& key, const I64& value)
    {
        if (const size_t pos = _dictionary.find(key);
            pos == Npos)
            _dictionary.insert(key, new IntegerType(value));
    }

    void ObjectType::insert(const String& key, const float& value)
    {
        insert(key, (double)value);
    }

    void ObjectType::insert(const String& key, const double& value)
    {
        if (const size_t pos = _dictionary.find(key);
            pos == Npos)
            _dictionary.insert(key, new DoubleType(value));
    }

    void ObjectType::insert(const String& key, const bool& value)
    {
        if (const size_t pos = _dictionary.find(key);
            pos == Npos)
            _dictionary.insert(key, new BoolType(value));
    }

    void ObjectType::insert(const String& key,
                            const void*   value)
    {
        if (const size_t pos = _dictionary.find(key);
            pos == Npos)
            _dictionary.insert(key, new PointerType(value));
    }

    void ObjectType::floatArray(const String& key, float** dest, int max)
    {
        String str;
        string(str, key);

        StringArray arr;
        Su::splitRejectEmpty(arr, str, ',');

        max = Min<int>(max, (int)arr.size());

        if (dest)
        {
            float*& rp = (float*&)*dest;
            for (int i = 0; i < max; ++i)
                rp[i] = Char::toFloat(arr[i]);
        }
    }

    void ObjectType::toString(StringBuilder& dest)
    {
        dest.write('{');

        bool first = true;

        for (const auto& it : _dictionary)
        {
            if (!first)
                dest.write(',');
            else
                first = false;
            dest.write('"');
            dest.write(it.first);
            dest.write('"');
            dest.write(':');
            dest.write(it.second->toString());
        }
        dest.write('}');
    }

}  // namespace Rt2::Json

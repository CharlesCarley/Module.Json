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
#ifndef _skJsonValue_h_
#define _skJsonValue_h_

#include "Utils/skString.h"
#include "Utils/skStringConverter.h"

class skJsonArray;
class skJsonObject;

/// \ingroup Json
///
/// Base class for all json objects.
class skJsonType
{
public:
    enum class Type
    {
        /// skJsonType undefined or an error
        UNDEFINED,
        /// skJsonArray
        ARRAY,
        /// skJsonBoolean
        BOOLEAN,
        /// skJsonDouble
        DOUBLE,
        /// skJsonInteger
        INTEGER,
        /// skJsonObject
        OBJECT,
        /// skJsonString
        STRING,
        /// null
        POINTER
    };

protected:
    /// Raw string value extracted from the .json file
    skString m_value;

    /// Holder for the class type code
    Type m_type;

    /// <summary>
    /// Primary constructor, protected to prevent calling it directly.
    /// </summary>
    /// <param name="type"></param>
    explicit skJsonType(const skJsonType::Type& type) :
        m_type(type)
    {
    }

    /// <summary>
    /// Default constructor, protected to prevent calling it directly.
    /// </summary>
    skJsonType() :
        m_type(Type::UNDEFINED)
    {
    }

public:
    virtual ~skJsonType()
    {
    }

    /// <summary>
    /// Explicitly set the internal string from the json file
    /// </summary>
    /// <param name="str">The value to assign to the internal string.</param>
    void setValue(const skString& str)
    {
        m_value.assign(str);
    }

    /// Provides access to the underlying value as a string.
    const skString& getString(void) const
    {
        return m_value;
    }

    /// Converts the underlying string to an integer or returns the default
    /// parameter if the internal type is not a Type::INTEGER.
    int getInteger(int defaultValue = -1) const
    {
        if (m_type == Type::INTEGER)
            return skStringConverter::toInt(m_value);
        return defaultValue;
    }

    /// Converts the underlying string to an integer or returns the default
    /// parameter if the internal type is not a Type::INTEGER.
    SKint64 getInteger64(SKint64 defaultValue = -1) const
    {
        if (m_type == Type::INTEGER)
            return skStringConverter::toInt64(m_value);
        return defaultValue;
    }

    /// Converts the underlying string to an double or returns the default
    /// parameter if the internal type is not a Type::DOUBLE.
    double getDouble(double defaultValue = 0.0) const
    {
        if (m_type == Type::DOUBLE)
            return skStringConverter::toDouble(m_value);
        return defaultValue;
    }

    /// \brief Converts the underlying string to a bool.
    ///
    /// Returns the default parameter if the internal type is not a Type::BOOLEAN.
    bool getBoolean(bool defaultValue = false) const
    {
        if (m_type == Type::BOOLEAN)
            return skStringConverter::toBool(m_value);
        return defaultValue;
    }

    /// <summary>
    /// Attempts to cast to an array
    /// </summary>
    /// <returns>skJsonArray or null if the type is not an array</returns>
    skJsonArray* asArray(void);

    /// <summary>
    /// Attempts to cast to an object
    /// </summary>
    /// <returns>skJsonObject or null if the type is not an object</returns>
    skJsonObject* asObject(void);

    /// <returns>true if the type is a string</returns>
    bool isString(void) const
    {
        return m_type == Type::STRING;
    }

    /// <returns>true if the type is an integer </returns>
    bool isInteger(void) const
    {
        return m_type == Type::INTEGER;
    }

    /// <returns>true if the type is a double</returns>
    bool isDouble(void) const
    {
        return m_type == Type::DOUBLE;
    }

    /// <returns>true if the type is a bool</returns>
    bool isBoolean(void) const
    {
        return m_type == Type::BOOLEAN;
    }

    /// <returns>true if the type is an object</returns>
    bool isObject(void) const
    {
        return m_type == Type::OBJECT;
    }

    /// <returns>true if the type is an array</returns>
    bool isArray(void) const
    {
        return m_type == Type::ARRAY;
    }

    /// <summary>
    /// Provides access to the type code
    /// </summary>
    /// <returns> skJsonType::Type</returns>
    Type getType(void) const
    {
        return m_type;
    }
};

#endif  //_skJsonValue_h_

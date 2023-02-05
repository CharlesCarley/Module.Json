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

#include "Utils/Char.h"
#include "Utils/Definitions.h"
#include "Utils/String.h"
#include "Utils/StringBuilder.h"

namespace Rt2::Json
{

    class ArrayType;
    class ObjectType;

    /// \ingroup Json
    ///
    /// Base class for all json objects.
    class Type
    {
    public:
        enum ClassType
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
        String _value;

        /// Holder for the class type code
        const ClassType _type;

        /// <summary>
        /// Primary constructor, protected to prevent calling it directly.
        /// </summary>
        /// <param name="type"></param>
        explicit Type(const ClassType& type) :
            _type(type)
        {
        }

        /// <summary>
        /// Default constructor, protected to prevent calling it directly.
        /// </summary>
        Type() :
            _type(UNDEFINED)
        {
        }

        virtual void notifyStringChanged()
        {
        }

        virtual void notifyValueChanged()
        {
        }

    public:
        virtual ~Type() = default;

        /// <summary>
        /// Explicitly set the internal string from a memory string
        /// </summary>
        /// <param name="mem">The value to assign to the internal string.</param>
        void setValue(const String& mem)
        {
            _value.assign(mem.c_str(), mem.size());
            notifyStringChanged();
        }

        /// Provides access to the underlying value as a string.
        const String& getString(void) const
        {
            return _value;
        }

        /// Converts the underlying string to an integer or returns the default
        /// parameter if the internal type is not a INTEGER.
        I16 getInt16(const I16 defaultValue = -1) const
        {
            if (_type == INTEGER)
                return Char::toInt16(_value, defaultValue);
            return defaultValue;
        }

        /// Converts the underlying string to an integer or returns the default
        /// parameter if the internal type is not a INTEGER.
        I32 getInt32(const I32 defaultValue = -1) const
        {
            if (_type == INTEGER)
                return Char::toInt32(_value, defaultValue);
            return defaultValue;
        }

        /// Converts the underlying string to an integer or returns the default
        /// parameter if the internal type is not a INTEGER.
        I64 getInt64(const I64 defaultValue = -1) const
        {
            if (_type == INTEGER)
                return Char::toInt64(_value, defaultValue);
            return defaultValue;
        }

        /// Converts the underlying string to an integer or returns the default
        /// parameter if the internal type is not a INTEGER.
        U16 getUint16(const U16 defaultValue = -1) const
        {
            if (_type == INTEGER)
                return Char::toUint16(_value, defaultValue);
            return defaultValue;
        }

        /// Converts the underlying string to an integer or returns the default
        /// parameter if the internal type is not a INTEGER.
        U32 getUint32(const U32 defaultValue = -1) const
        {
            if (_type == INTEGER)
                return Char::toUint32(_value, defaultValue);
            return defaultValue;
        }

        /// Converts the underlying string to an integer or returns the default
        /// parameter if the internal type is not a INTEGER.
        U64 getUint64(const U64 defaultValue = -1) const
        {
            if (_type == INTEGER)
                return Char::toUint64(_value, defaultValue);
            return defaultValue;
        }

        /// Converts the underlying string to an double or returns the default
        /// parameter if the internal type is not a DOUBLE.
        double getDouble(const double defaultValue = 0.0) const
        {
            if (_type == DOUBLE)
                return Char::toDouble(_value);
            return defaultValue;
        }

        /// \brief Converts the underlying string to a bool.
        ///
        /// Returns the default parameter if the internal type is not a BOOLEAN.
        bool getBoolean(const bool defaultValue = false) const
        {
            if (_type == BOOLEAN)
                return Char::toBool(_value);
            return defaultValue;
        }

        /// <summary>
        /// Attempts to cast to an array
        /// </summary>
        /// <returns>skJsonArray or null if the type is not an array</returns>
        ArrayType* asArray(void);

        /// <summary>
        /// Attempts to cast to an object
        /// </summary>
        /// <returns>skJsonObject or null if the type is not an object</returns>
        ObjectType* asObject(void);

        /// <returns>true if the type is a string</returns>
        bool isString(void) const
        {
            return _type == STRING;
        }

        /// <returns>true if the type is an integer </returns>
        bool isInteger(void) const
        {
            return _type == INTEGER;
        }

        /// <returns>true if the type is a double</returns>
        bool isDouble(void) const
        {
            return _type == DOUBLE;
        }

        /// <returns>true if the type is a bool</returns>
        bool isBoolean(void) const
        {
            return _type == BOOLEAN;
        }

        /// <returns>true if the type is an object</returns>
        bool isObject(void) const
        {
            return _type == OBJECT;
        }

        /// <returns>true if the type is an array</returns>
        bool isArray(void) const
        {
            return _type == ARRAY;
        }

        /// <summary>
        /// Provides access to the type code
        /// </summary>
        /// <returns> skJsonType::Type</returns>
        ClassType getType() const
        {
            return _type;
        }

        /// <returns>Returns a string representation of the object. </returns>
        String toString()
        {
            StringBuilder sb;
            toString(sb);
            return sb.toString();
        }

        /// <summary>
        /// Returns a string representation of the object.
        /// </summary>
        /// <param name="dest">A destination reference</param>
        virtual void toString(String& dest)
        {
            StringBuilder sb;
            toString(sb);
            sb.toString(dest);
        }

        /// <summary>
        /// Returns a string representation of the object.
        /// </summary>
        /// <param name="dest">A destination reference</param>
        virtual void toString(StringBuilder& dest) = 0;
    };
}  // namespace Rt2::Json

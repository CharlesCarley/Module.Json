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
#include "Utils/HashMap.h"
#include "Utils/String.h"


namespace Rt2::Json
{

    class ObjectType final : public Type
    {
    public:
        using Dictionary = HashTable<String, Type*>;

    protected:
        Dictionary _dictionary;

    public:
        ObjectType();
        ~ObjectType() override;

        /// <summary>
        /// Inserts a Type object into the dictionary with a lookup key.
        /// </summary>
        /// <param name="key">The key parameter should be a unique lookup name
        /// within the context of this object.</param>
        /// <param name="value">value is the object that is to be
        /// stored.</param>
        ///
        /// <remarks>
        ///
        /// Represents storage for the following json code fragment
        /// <code>
        /// "A" : "Hello World"  => obj->insert("A", new StringType("Hello World"))
        /// </code>
        /// </remarks>
        void insert(const String& key, Type* value);

        /// <summary>
        /// Inserts a 16-bit signed integer into the dictionary by the supplied lookup
        /// key.
        /// </summary>
        /// <param name="key">
        /// key should be a unique lookup name within the context of
        /// this object.
        /// </param>
        /// <param name="value">value is the value that is to be
        /// stored.</param>
        void insert(const String& key, const I16& value);

        /// <summary>
        /// Inserts a 32-bit signed integer into the dictionary by the supplied lookup
        /// key.
        /// </summary>
        /// <param name="key">
        /// key should be a unique lookup name within the context of
        /// this object.
        /// </param>
        /// <param name="value">value is the value that is to be
        /// stored.</param>
        void insert(const String& key, const I32& value);

        /// <summary>
        /// Inserts a 64-bit signed integer into the dictionary by the supplied lookup
        /// key.
        /// </summary>
        /// <param name="key">
        /// key should be a unique lookup name within the context of
        /// this object.
        /// </param>
        /// <param name="value">value is the value that is to be
        /// stored.</param>
        void insert(const String& key, const I64& value);

        /// <summary>
        /// Inserts a floating point value into the dictionary by the supplied lookup
        /// key.
        /// </summary>
        /// <param name="key">
        /// key should be a unique lookup name within the context of
        /// this object.
        /// </param>
        /// <param name="value">value is the value that is to be
        /// stored.</param>
        void insert(const String& key, const float& value);

        /// <summary>
        /// Inserts a double precision value into the dictionary by the supplied lookup
        /// key.
        /// </summary>
        /// <param name="key">
        /// key should be a unique lookup name within the context of
        /// this object.
        /// </param>
        /// <param name="value">value is the value that is to be
        /// stored.</param>
        void insert(const String& key, const double& value);

        /// <summary>
        /// Inserts a boolean value into the dictionary by the supplied lookup
        /// key.
        /// </summary>
        /// <param name="key">
        /// key should be a unique lookup name within the context of
        /// this object.
        /// </param>
        /// <param name="value">value is the value that is to be
        /// stored.</param>
        void insert(const String& key, const bool& value);

        /// <summary>
        /// Inserts a pointer address into the dictionary by the supplied lookup
        /// key.
        /// </summary>
        /// <param name="key">
        /// key should be a unique lookup name within the context of
        /// this object.
        /// </param>
        /// <param name="value">value is the value that is to be
        /// stored.</param>
        void insert(const String& key, const void* value);

        /// <summary>
        /// Returns true if the object has a field with the supplied key.
        /// </summary>
        /// <param name="key">The name of the object to search for.</param>
        /// <returns></returns>
        bool hasKey(const String& key) const;

        /// <summary>
        /// Gets the Json object that is associated with the key.
        /// </summary>
        /// <param name="key">The name of the object to search for.</param>
        /// <returns>The object if the object is found otherwise returns null</returns>
        Type* find(const String& key);

        /// <summary>
        /// Gets the requested string from the dictionary
        /// </summary>
        /// <param name="dest">The destination storage variable.</param>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        /// <returns>The value for the requested key otherwise def</returns>
        void string(String& dest, const String& key, const String& def = "");

        /// <summary>
        /// Gets the requested signed 64-bit integer from the dictionary
        /// </summary>
        /// <param name="dest">The destination storage variable.</param>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        /// <returns>The value for the requested key otherwise def</returns>
        void integer(I64& dest, const String& key, const I64& def = -1);

        /// <summary>
        /// Gets the requested signed 32-bit integer from the dictionary
        /// </summary>
        /// <param name="dest">The destination storage variable.</param>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        /// <returns>The value for the requested key otherwise def</returns>
        void integer(I32& dest, const String& key, const I32& def = -1);

        /// <summary>
        /// Gets the requested signed 16-bit integer from the dictionary
        /// </summary>
        /// <param name="dest">The destination storage variable.</param>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        /// <returns>The value for the requested key otherwise def</returns>
        void integer(I16& dest, const String& key, const I16& def = -1);

        /// <summary>
        /// Gets the requested signed 16-bit integer from the dictionary
        /// </summary>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        /// <returns></returns>
        I16 i16(const String& key, const I16& def = -1)
        {
            I16 val;
            integer(val, key, def);
            return val;
        }

        /// <summary>
        /// Gets the requested signed 32-bit integer from the dictionary
        /// </summary>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        /// <returns></returns>
        I32 i32(const String& key, const I32& def = -1)
        {
            I32 val;
            integer(val, key, def);
            return val;
        }

        /// <summary>
        /// Gets the requested signed 64-bit integer from the dictionary
        /// </summary>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        /// <returns></returns>
        I64 i64(const String& key, const I64& def = -1)
        {
            I64 val;
            integer(val, key, def);
            return val;
        }

        /// <summary>
        /// Gets the requested boolean from the dictionary
        /// </summary>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        bool boolean(const String& key, bool def = false);

        /// <summary>
        /// Gets the requested double precision value from the dictionary
        /// </summary>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        double r64(const String& key, double def = 0.0);

        /// <summary>
        /// Gets the requested single precision value from the dictionary
        /// </summary>
        /// <param name="key">The key to get</param>
        /// <param name="def">The default value if the key is not found.</param>
        float r32(const String& key, float def = 0.0);

        /// <summary>
        /// Returns a string representation of the object.
        /// </summary>
        /// <param name="dest">A destination reference</param>
        void toString(String& dest) override
        {
            Type::toString(dest);
        }

        /// <summary>
        /// Returns a string representation of the object.
        /// </summary>
        /// <param name="dest">A destination reference</param>
        void toString(StringBuilder& dest) override;

        /// <summary>
        /// Parses the string stored in key as a float array
        /// </summary>
        /// <param name="key"></param>
        /// <param name="dest"></param>
        /// <param name="max"></param>
        void floatArray(const String& key, float** dest, int max);

        Dictionary& dictionary()
        {
            return _dictionary;
        }

        const Dictionary& dictionary() const
        {
            return _dictionary;
        }
    };
}  // namespace Rt2::Json

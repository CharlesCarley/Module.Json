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
#include "Utils/Array.h"

namespace Rt2::Json
{

    /// <summary>
    /// Array type access
    /// </summary>
    class ArrayType final : public Type
    {
    public:
        /// <summary>
        /// Array of json types.
        /// </summary>
        typedef Array<Type*> TypeArray;

    private:
        TypeArray _array;

    public:
        /// <summary>
        /// Default array constructor.
        /// </summary>
        ArrayType();

        ~ArrayType() override;

        /// <summary>
        /// Appends a value to the array.
        /// </summary>
        /// <param name="value">skJsonType</param>
        void add(Type* value);

        /// <summary>
        /// Appends a 16-bit signed integer value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const I16& value);

        /// <summary>
        /// Appends a 32-bit signed integer value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const I32& value);

        /// <summary>
        /// Appends a 64-bit signed integer value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const I64& value);

        /// <summary>
        /// Appends a 16-bit unsigned integer value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const U16& value);

        /// <summary>
        /// Appends a 32-bit unsigned integer value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const U32& value);

        /// <summary>
        /// Appends a 64-bit unsigned integer value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const U64& value);

        /// <summary>
        /// Appends a boolean value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(bool value);

        /// <summary>
        /// Appends a double precision value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const double& value);

        /// <summary>
        /// Appends a single precision value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const float& value);

        /// <summary>
        /// Appends a string value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const String& value);

        /// <summary>
        /// Appends a pointer value to the array.
        /// </summary>
        /// <param name="value">The value to add to the array</param>
        void add(const void* value);

        /// <summary>
        /// Returns the number of elements in the array.
        /// </summary>
        U32 size(void) const
        {
            return _array.size();
        }

        /// <summary>
        /// Get the array element at the supplied index.
        /// </summary>
        /// <param name="i">position to access</param>
        /// <returns>skJsonType or null if the index is out of bounds.</returns>
        Type* at(const U32 i)
        {
            return i < _array.size() ? _array.at(i) : nullptr;
        }

        /// <summary>
        /// Attempts to convert the type at the supplied index to an integer.
        /// </summary>
        /// <param name="i">The array position to access</param>
        /// <param name="def">Is the return value on any error condition.</param>
        /// <returns>An integer or the supplied default if the index is null or out of bounds. </returns>
        I16 int16(const U32 i, const I16 def = -1)
        {
            const Type* type = at(i);
            if (!type || !type->isInteger())
                return def;
            return type->getInt16(def);
        }

        /// <summary>
        /// Attempts to convert the type at the supplied index to an integer.
        /// </summary>
        /// <param name="i">The array position to access</param>
        /// <param name="def">Is the return value on any error condition.</param>
        /// <returns>An integer or the supplied default if the index is null or out of bounds. </returns>
        I32 int32(const U32 i, const I32 def = -1)
        {
            const Type* type = at(i);
            if (!type || !type->isInteger())
                return def;
            return type->getInt32(def);
        }

        /// <summary>
        /// Attempts to convert the type at the supplied index to an integer.
        /// </summary>
        /// <param name="i">The array position to access</param>
        /// <param name="def">Is the return value on any error condition.</param>
        /// <returns>An integer or the supplied default if the index is null or out of bounds. </returns>
        I64 int64(const U32 i, const I64 def = -1)
        {
            const Type* type = at(i);
            if (!type || !type->isInteger())
                return def;
            return type->getInt64(def);
        }

        void toString(String& dest) override
        {
            Type::toString(dest);
        }

        void toString(StringBuilder& dest) override;
    };
}  // namespace Rt2::Json

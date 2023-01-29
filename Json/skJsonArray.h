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

#include "Utils/skArray.h"
#include "skJsonType.h"

/// <summary>
/// Array type access
/// </summary>
class skJsonArray final : public skJsonType
{
public:
    /// <summary>
    /// Array of json types.
    /// </summary>
    typedef skArray<skJsonType*> Array;

private:
    Array m_array;

public:
    /// <summary>
    /// Default array constructor.
    /// </summary>
    skJsonArray();

    ~skJsonArray() override;

    /// <summary>
    /// Appends a value to the array.
    /// </summary>
    /// <param name="value">skJsonType</param>
    void add(skJsonType* value);

    /// <summary>
    /// Appends a 16-bit signed integer value to the array.
    /// </summary>
    /// <param name="value">The value to add to the array</param>
    void add(const SKint16& value);

    /// <summary>
    /// Appends a 32-bit signed integer value to the array.
    /// </summary>
    /// <param name="value">The value to add to the array</param>
    void add(const SKint32& value);

    /// <summary>
    /// Appends a 64-bit signed integer value to the array.
    /// </summary>
    /// <param name="value">The value to add to the array</param>
    void add(const SKint64& value);

    /// <summary>
    /// Appends a 16-bit unsigned integer value to the array.
    /// </summary>
    /// <param name="value">The value to add to the array</param>
    void add(const SKuint16& value);

    /// <summary>
    /// Appends a 32-bit unsigned integer value to the array.
    /// </summary>
    /// <param name="value">The value to add to the array</param>
    void add(const SKuint32& value);

    /// <summary>
    /// Appends a 64-bit unsigned integer value to the array.
    /// </summary>
    /// <param name="value">The value to add to the array</param>
    void add(const SKuint64& value);

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
    void add(const skString& value);

    /// <summary>
    /// Appends a pointer value to the array.
    /// </summary>
    /// <param name="value">The value to add to the array</param>
    void add(const void* value);

    /// <summary>
    /// Returns the number of elements in the array.
    /// </summary>
    SKuint32 size(void) const
    {
        return m_array.size();
    }

    /// <summary>
    /// Get the array element at the supplied index.
    /// </summary>
    /// <param name="i">position to access</param>
    /// <returns>skJsonType or null if the index is out of bounds.</returns>
    skJsonType* at(const SKuint32 i)
    {
        return i < m_array.size() ? m_array.at(i) : nullptr;
    }

    /// <summary>
    /// Attempts to convert the type at the supplied index to an integer.
    /// </summary>
    /// <param name="i">The array position to access</param>
    /// <param name="def">Is the return value on any error condition.</param>
    /// <returns>An integer or the supplied default if the index is null or out of bounds. </returns>
    SKint16 int16(const SKuint32 i, const SKint16 def = -1)
    {
        skJsonType* type = at(i);
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
    SKint32 int32(const SKuint32 i, const SKint32 def = -1)
    {
        skJsonType* type = at(i);
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
    SKint64 int64(const SKuint32 i, const SKint64 def = -1)
    {
        skJsonType* type = at(i);
        if (!type || !type->isInteger())
            return def;
        return type->getInt64(def);
    }

    void toString(skString& dest) override
    {
        skJsonType::toString(dest);
        
    }

    void toString(skStringBuilder& dest) override;

    Array::Iterator iterator()
    {
        return m_array.iterator();
    }
};

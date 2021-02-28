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
#ifndef _skJsonArray_h_
#define _skJsonArray_h_

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

    virtual ~skJsonArray();

    /// <summary>
    /// Appends a value to the array.
    /// </summary>
    /// <param name="value">skJsonType</param>
    void add(skJsonType* value);

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
    /// <param name="defaultValue">Is the return value on any error condition.</param>
    /// <returns>An integer or the supplied defaultValue if the index is
    /// null or out of bounds.
    /// </returns>
    int intAt(const SKuint32 i, int defaultValue = -1)
    {
        skJsonType* type = at(i);
        if (!type || !type->isInteger())
            return defaultValue;
        return type->getInteger(defaultValue);
    }
};

#endif  //_skJsonArray_h_

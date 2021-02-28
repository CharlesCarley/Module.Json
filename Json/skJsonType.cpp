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
#include "skJsonType.h"
#include "skJsonArray.h"
#include "skJsonObject.h"

/// <summary>
/// Attempts to cast to an array
/// </summary>
/// <returns>skJsonArray or null if the type is not an array</returns>
skJsonArray* skJsonType::asArray(void)
{
    if (m_type == Type::ARRAY)
        return reinterpret_cast<skJsonArray*>(this);
    return nullptr;
}

/// <summary>
/// Attempts to cast to an object
/// </summary>
/// <returns>skJsonObject or null if the type is not an object</returns>
skJsonObject* skJsonType::asObject(void)
{
    if (m_type == Type::OBJECT)
        return reinterpret_cast<skJsonObject*>(this);
    return nullptr;
}

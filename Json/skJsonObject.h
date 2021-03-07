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
#ifndef _skJsonObject_h_
#define _skJsonObject_h_

#include "Utils/skMap.h"
#include "skJsonType.h"

/// \ingroup Json
class skJsonObject final : public skJsonType
{
public:
    typedef skHashTable<skString, skJsonType*> Dictionary;

protected:
    Dictionary m_dictionary;

public:
    skJsonObject();
    virtual ~skJsonObject();

    /// <summary>
    /// Inserts a skJsonType object into the dictionary by the supplied lookup
    /// key.
    /// </summary>
    /// <param name="key">key should be a unique lookup name within the context of
    /// this object.</param> <param name="value">value is the value that is to be
    /// stored.</param>
    ///
    /// <code>
    /// "A" : "Hello World"
    /// </code>
    ///
    void insert(const skString& key, skJsonType* value);

    /// <summary>
    ///
    /// </summary>
    /// <param name="key">a</param>
    /// <returns></returns>
    bool hasKey(const skString& key) const;

    /// <summary>
    ///
    /// </summary>
    /// <param name="key">aq</param>
    /// <returns></returns>
    skJsonType* getValue(const skString& key);

    /// <summary>
    /// Gets the requested string from the dictionary
    /// </summary>
    /// <param name="dest">The destination storage variable.</param>
    /// <param name="key">The key to get</param>
    /// <param name="def">The default value if the key is not found.</param>
    /// <returns>The value for the requested key otherwise def</returns>
    void getValueString(skString& dest, const skString& key, const skString& def = "");


    /// <summary>
    /// Gets the requested signed 64-bit integer from the dictionary
    /// </summary>
    /// <param name="dest">The destination storage variable.</param>
    /// <param name="key">The key to get</param>
    /// <param name="def">The default value if the key is not found.</param>
    /// <returns>The value for the requested key otherwise def</returns>
    void getValueInt(SKint64& dest, const skString& key, const SKint64& def = -1);

    /// <summary>
    /// Gets the requested signed 32-bit integer from the dictionary
    /// </summary>
    /// <param name="dest">The destination storage variable.</param>
    /// <param name="key">The key to get</param>
    /// <param name="def">The default value if the key is not found.</param>
    /// <returns>The value for the requested key otherwise def</returns>
    void getValueInt(SKint32& dest, const skString& key, const SKint32& def = -1);


    /// <summary>
    /// Gets the requested signed 32-bit integer from the dictionary
    /// </summary>
    /// <param name="dest">The destination storage variable.</param>
    /// <param name="key">The key to get</param>
    /// <param name="def">The default value if the key is not found.</param>
    /// <returns>The value for the requested key otherwise def</returns>
    void getValueInt(SKint16& dest, const skString& key, const SKint16& def = -1);
};

#endif  //_skJsonObject_h_

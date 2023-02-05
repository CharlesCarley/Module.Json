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

#include "Utils/String.h"
namespace Rt2::Json
{

    /// \ingroup Json
    ///
    class Visitor
    {
    public:
        virtual ~Visitor() = default;

        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual Type* getRoot()
        {
            return nullptr;
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="last"></param>
        virtual void parseError(const Token& last)
        {
        }

        /// <summary>
        ///
        /// </summary>
        virtual void objectCreated()
        {
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="key"></param>
        /// <param name="valueType"></param>
        /// <param name="value"></param>
        virtual void keyValueParsed(const String&    key,
                                    const TokenType& valueType,
                                    const String&    value)
        {
        }

        /// <summary>
        ///
        /// </summary>
        virtual void objectFinished()
        {
        }

        /// <summary>
        ///
        /// </summary>
        virtual void arrayCreated()
        {
        }

        /// <summary>
        /// Called after an object was parsed in an array
        /// </summary>
        virtual void objectParsed(void)
        {
        }

        /// <summary>
        /// Called after an array was parsed in an array
        /// </summary>
        virtual void arrayParsed(void)
        {
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="value"></param>
        virtual void stringParsed(const String& value)
        {
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="value"></param>
        virtual void integerParsed(const String& value)
        {
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="value"></param>
        virtual void doubleParsed(const String& value)
        {
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="value"></param>
        virtual void booleanParsed(const String& value)
        {
        }

        /// <summary>
        ///
        /// </summary>
        /// <param name="value"></param>
        virtual void pointerParsed(const String& value)
        {
        }

        /// <summary>
        ///
        /// </summary>
        virtual void arrayFinished()
        {
        }
    };
}  // namespace Rt2::Json

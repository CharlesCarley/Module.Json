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

#include "Scanner.h"
#include "Token.h"

namespace Rt2::Json
{

    class Visitor;

    class Parser
    {
    private:
        Visitor* _visitor;
        bool     _owns;

        void parseObject(Scanner& scn, Token& tok);

        void parseArray(Scanner& scn, Token& tok);

        Type* parseCommon(Scanner& scanner);

    public:
        explicit Parser(Visitor* visitor = nullptr);
        ~Parser();

        /// <summary>
        /// Attempts to parse the file path as JSON.
        /// </summary>
        /// <param name="path">File system path</param>
        /// <returns>
        /// Returns a skJsonType object.
        ///
        /// If the root object in the script is opened with a bracket then a
        /// skJsonObject is returned. <code>
        /// {
        ///     "Key": "Value"
        /// }
        /// </code>
        /// If the root object in the script is opened with a brace
        /// then a skJsonArray is returned.
        /// <code> [1, 2, 3] </code>
        ///
        /// use skJsonType methods to determine the underlying type.
        /// </returns>
        Type* parse(const String& path);

        /// <summary>
        /// Attempts to parse the file path as JSON and expects an object to have been supplied.
        /// </summary>
        /// <param name="path">File system path</param>
        ObjectType* parseObject(const String& path);

        /// <summary>
        /// Attempts to parse the memory as JSON.
        /// </summary>
        /// <param name="src">Memory source</param>
        /// <param name="sizeInBytes">The size of the source memory in bytes</param>
        /// <returns>
        /// Returns a skJsonType object.
        ///
        /// If the root object in the script is opened with a bracket then a
        /// skJsonObject is returned. <code>
        /// {
        ///     "Key": "Value"
        /// }
        /// </code>
        /// If the root object in the script is opened with a brace
        /// then a skJsonArray is returned.
        /// <code> [1, 2, 3] </code>
        ///
        /// use skJsonType methods to determine the underlying type.
        /// </returns>
        Type* parse(const char* src, size_t sizeInBytes);
    };
}  // namespace Rt2::Json
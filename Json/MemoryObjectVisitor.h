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

#include "ObjectType.h"
#include "Token.h"
#include "Utils/Stack.h"
#include "Visitor.h"

namespace Rt2::Json
{
    /// <summary>
    /// Default visitor
    /// Creates type wrappers for parsed json types.
    /// </summary>
    class MemoryObjectVisitor final : public Visitor
    {
    public:
        typedef Stack<ObjectType*> ObjectStack;
        typedef Stack<ArrayType*>  ArrayStack;

    private:
        Type*       _root{nullptr};
        ObjectStack _objStack{};
        ArrayStack  _arrStack{};
        ObjectStack _finishedObjects{};
        ArrayStack  _finishedArrays{};

    public:
        MemoryObjectVisitor() = default;

        ~MemoryObjectVisitor() override;

        void clear();

        void parseError(const Token& last) override;

        Type* root() override;

        void arrayCreated() override;

        void objectCreated() override;

        void objectFinished() override;

        void arrayFinished() override;

        void keyValueParsed(const String&    key,
                            const TokenType& valueType,
                            const String&    value) override;

        void handleArrayType(Type* obj, const String& value);

        void objectParsed() override;

        void arrayParsed() override;

        void stringParsed(const String& value) override;

        void integerParsed(const String& value) override;

        void doubleParsed(const String& value) override;

        void booleanParsed(const String& value) override;

        void pointerParsed(const String& value) override;
    };

}  // namespace Rt2::Json
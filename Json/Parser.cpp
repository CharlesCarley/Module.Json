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
#include "Parser.h"
#include "ArrayType.h"
#include "BoolType.h"
#include "MemoryObjectVisitor.h"
#include "ObjectType.h"
#include "Scanner.h"
#include "Token.h"
#include "Utils/Stack.h"
#include "Visitor.h"

namespace Rt2::Json
{

    Parser::Parser(Visitor* visitor) :
        _visitor(visitor),
        _owns(visitor == nullptr)
    {
        if (_visitor == nullptr)
            _visitor = new MemoryObjectVisitor();
    }

    Parser::~Parser()
    {
        if (_owns)
            delete _visitor;
    }

    Type* Parser::parseCommon(Scanner& scanner)
    {
        Token tok;
        scanner.scan(tok);

        Type* root;
        if (tok.type() == JT_L_BRACKET)
        {
            parseObject(scanner, tok);
            root = _visitor->root();
        }
        else if (tok.type() == JT_L_BRACE)
        {
            parseArray(scanner, tok);
            root = _visitor->root();
        }
        else
            root = nullptr;
        return root;
    }

    Type* Parser::parse(const String& path)
    {
        Scanner scn;
        scn.open(path);

        if (!scn.isOpen())
        {
            Console::writeError("failed to open the supplied file: ", path.c_str());
            return nullptr;
        }
        return parseCommon(scn);
    }

    ObjectType* Parser::parseObject(const String& path)
    {
        Type* object = parse(path);
        return object ? object->asObject() : nullptr;
    }

    Type* Parser::parse(const char* src, const size_t sizeInBytes)
    {
        Scanner scn;
        scn.open(src, sizeInBytes);

        if (!scn.isOpen())
        {
            Console::writeError("failed to open the supplied memory file");
            return nullptr;
        }
        return parseCommon(scn);
    }

    void Parser::parseObject(Scanner& scn, Token& tok)
    {
        _visitor->objectCreated();
        Token t1, t2;

        while (tok.type() != JT_R_BRACKET)
        {
            scn.scan(t1);

            if (t1.type() == JT_R_BRACKET)
            {
                break;  // empty
            }

            if (t1.type() != JT_STRING)
            {
                _visitor->parseError(t1);
                return;
            }

            scn.scan(t2);
            if (t2.type() != JT_COLON)
            {
                _visitor->parseError(t2);
                return;
            }
            scn.scan(t2);

            TokenType type = t2.type();
            switch (type)
            {
            case JT_L_BRACE:
                parseArray(scn, t2);
                break;
            case JT_L_BRACKET:
                parseObject(scn, t2);
                break;
            case JT_STRING:
            case JT_NULL:
            case JT_BOOL:
            case JT_NUMBER:
            case JT_INTEGER:
                break;
            case JT_UNDEFINED:
            case JT_COLON:
            case JT_COMMA:
            case JT_R_BRACE:
            case JT_R_BRACKET:
                _visitor->parseError(t2);
                return;
            }

            _visitor->keyValueParsed(t1.value(), type, t2.value());

            scn.scan(tok);
            if (tok.type() == JT_NULL)
            {
                _visitor->parseError(tok);
                return;
            }
        }

        _visitor->objectFinished();
    }

    void Parser::parseArray(Scanner& scn, Token& tok)
    {
        _visitor->arrayCreated();
        Token t1;

        while (tok.type() != JT_R_BRACE)
        {
            scn.scan(t1);
            if (t1.type() == JT_R_BRACE)
                break;  // empty

            switch (t1.type())
            {
            case JT_L_BRACE:
                parseArray(scn, t1);
                _visitor->arrayParsed();
                break;
            case JT_L_BRACKET:
                parseObject(scn, t1);
                _visitor->objectParsed();
                break;
            case JT_STRING:
                _visitor->stringParsed(t1.value());
                break;
            case JT_NULL:
                _visitor->pointerParsed(t1.value());
                break;
            case JT_BOOL:
                _visitor->booleanParsed(t1.value());
                break;
            case JT_NUMBER:
                _visitor->doubleParsed(t1.value());
                break;
            case JT_INTEGER:
                _visitor->integerParsed(t1.value());
                break;
            case JT_UNDEFINED:
            case JT_COLON:
            case JT_COMMA:
            case JT_R_BRACE:
            case JT_R_BRACKET:
                _visitor->parseError(t1);
                return;
            }

            scn.scan(tok);
            if (tok.type() == JT_NULL)
            {
                _visitor->parseError(tok);
                return;
            }
        }
        _visitor->arrayFinished();
    }
}  // namespace Rt2::Json
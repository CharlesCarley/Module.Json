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
#include "Utils/Stack.h"
#include "ArrayType.h"
#include "BoolType.h"
#include "DoubleType.h"
#include "IntegerType.h"
#include "ObjectType.h"
#include "PointerType.h"
#include "Scanner.h"
#include "StringType.h"
#include "Token.h"
#include "Visitor.h"

namespace Rt2::Json
{
    /// <summary>
    /// Default visitor
    /// Creates type wrappers for parsed json types.
    /// </summary>
    class skStoreObjectVisitor final : public Visitor
    {
    public:
        typedef Stack<ObjectType*> ObjectStack;
        typedef Stack<ArrayType*>  ArrayStack;

    private:
        Type* _root{nullptr};
        ObjectStack _objStack{};
        ArrayStack  _arrStack{};
        ObjectStack _finishedObjects{};
        ArrayStack  _finishedArrays{};

    public:
        skStoreObjectVisitor() = default;

        ~skStoreObjectVisitor() override
        {
            clear();

            delete _root;
            _root = nullptr;
        }

        void clear()
        {
            while (!_arrStack.empty())
            {
                delete _arrStack.top();
                _arrStack.pop();
            }
            while (!_objStack.empty())
            {
                delete _objStack.top();
                _objStack.pop();
            }
            while (!_finishedObjects.empty())
            {
                delete _finishedObjects.top();
                _finishedObjects.pop();
            }
            while (!_finishedObjects.empty())
            {
                delete _finishedObjects.top();
                _finishedObjects.pop();
            }
        }

        void parseError(const Token& last) override
        {
            Console::writeError("Parse error: ", last.getValue().c_str());
            clear();
        }

        Type* getRoot() override
        {
            if (!_finishedObjects.empty())
            {
                _root = _finishedObjects.top();
                _finishedObjects.pop();
            }
            else if (!_finishedArrays.empty())
            {
                _root = _finishedArrays.top();
                _finishedArrays.pop();
            }
            return _root;
        }

        void arrayCreated() override
        {
            _arrStack.push(new ArrayType());
        }

        void objectCreated() override
        {
            _objStack.push(new ObjectType());
        }

        void objectFinished() override
        {
            if (!_objStack.empty())
            {
                _finishedObjects.push(_objStack.top());
                _objStack.pop();
            }
        }

        void arrayFinished() override
        {
            if (!_arrStack.empty())
            {
                _finishedArrays.push(_arrStack.top());
                _arrStack.pop();
            }
        }

        void keyValueParsed(const String&          key,
                            const TokenType& valueType,
                            const String&          value) override
        {
            if (_objStack.empty())
            {
                Con::writeError("no object on the parse stack\n");
                return;
            }

            ObjectType* top = _objStack.top();

            Type* obj = nullptr;
            switch (valueType)
            {
            case TokenType::JT_L_BRACE:
                if (!_finishedArrays.empty())
                {
                    obj = _finishedArrays.top();
                    _finishedArrays.pop();
                }
                break;
            case TokenType::JT_L_BRACKET:
                if (!_finishedObjects.empty())
                {
                    obj = _finishedObjects.top();
                    _finishedObjects.pop();
                }
                break;
            case TokenType::JT_STRING:
                obj = new StringType();
                break;
            case TokenType::JT_NULL:
                obj = new PointerType();
                break;
            case TokenType::JT_BOOL:
                obj = new BoolType();
                break;
            case TokenType::JT_NUMBER:
                obj = new DoubleType();
                break;
            case TokenType::JT_INTEGER:
                obj = new IntegerType();
                break;
            case TokenType::JT_UNDEFINED:
            case TokenType::JT_COLON:
            case TokenType::JT_COMMA:
            case TokenType::JT_R_BRACE:
            case TokenType::JT_R_BRACKET:
                break;
            }

            if (obj != nullptr)
                obj->setValue(value);
            top->insert(key, obj);
        }

        void handleArrayType(Type* obj, const String& value)
        {
            if (obj != nullptr)
            {
                obj->setValue(value);
                auto* arrayObject = _arrStack.top();
                arrayObject->add(obj);
            }
        }

        void objectParsed() override
        {
            if (!_arrStack.empty() && !_finishedObjects.empty())
            {
                _arrStack.top()->add(_finishedObjects.top());
                _finishedObjects.pop();
            }
        }

        void arrayParsed() override
        {
            if (!_arrStack.empty() && !_finishedArrays.empty())
            {
                _arrStack.top()->add(_finishedArrays.top());
                _finishedArrays.pop();
            }
        }

        void stringParsed(const String& value) override
        {
            if (!_arrStack.empty())
                handleArrayType(new ArrayType(), value);
        }

        void integerParsed(const String& value) override
        {
            if (!_arrStack.empty())
                handleArrayType(new IntegerType(), value);
        }

        void doubleParsed(const String& value) override
        {
            if (!_arrStack.empty())
                handleArrayType(new DoubleType(), value);
        }

        void booleanParsed(const String& value) override
        {
            if (!_arrStack.empty())
                handleArrayType(new DoubleType(), value);
        }

        void pointerParsed(const String& value) override
        {
            if (!_arrStack.empty())
            {
                handleArrayType(new PointerType(), value);
            }
        }
    };


    Parser::Parser(Visitor* visitor) :
        _visitor(visitor),
        _owns(visitor == nullptr)
    {
        if (_visitor == nullptr)
            _visitor = new skStoreObjectVisitor();
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
        if (tok.getType() == TokenType::JT_L_BRACKET)
        {
            parseObject(scanner, tok);
            root = _visitor->getRoot();
        }
        else if (tok.getType() == TokenType::JT_L_BRACE)
        {
            parseArray(scanner, tok);
            root = _visitor->getRoot();
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
            Con::writeError("failed to open the supplied file: ", path.c_str());
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
            Con::writeError("failed to open the supplied memory file");
            return nullptr;
        }
        return parseCommon(scn);
    }

    void Parser::parseObject(Scanner& scn, Token& tok)
    {
        _visitor->objectCreated();
        Token t1, t2;

        while (tok.getType() != TokenType::JT_R_BRACKET)
        {
            scn.scan(t1);

            if (t1.getType() == TokenType::JT_R_BRACKET)
            {
                break;  // empty
            }

            if (t1.getType() != TokenType::JT_STRING)
            {
                _visitor->parseError(t1);
                return;
            }

            scn.scan(t2);
            if (t2.getType() != TokenType::JT_COLON)
            {
                _visitor->parseError(t2);
                return;
            }
            scn.scan(t2);

            TokenType type = t2.getType();
            switch (type)
            {
            case TokenType::JT_L_BRACE:
                parseArray(scn, t2);
                break;
            case TokenType::JT_L_BRACKET:
                parseObject(scn, t2);
                break;
            case TokenType::JT_STRING:
            case TokenType::JT_NULL:
            case TokenType::JT_BOOL:
            case TokenType::JT_NUMBER:
            case TokenType::JT_INTEGER:
                break;
            case TokenType::JT_UNDEFINED:
            case TokenType::JT_COLON:
            case TokenType::JT_COMMA:
            case TokenType::JT_R_BRACE:
            case TokenType::JT_R_BRACKET:
                _visitor->parseError(t2);
                return;
            }

            _visitor->keyValueParsed(t1.getValue(), type, t2.getValue());

            scn.scan(tok);
            if (tok.getType() == TokenType::JT_NULL)
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

        while (tok.getType() != TokenType::JT_R_BRACE)
        {
            scn.scan(t1);
            if (t1.getType() == TokenType::JT_R_BRACE)
            {
                break;  // empty
            }

            switch (t1.getType())
            {
            case TokenType::JT_L_BRACE:
                parseArray(scn, t1);
                _visitor->arrayParsed();
                break;
            case TokenType::JT_L_BRACKET:
                parseObject(scn, t1);
                _visitor->objectParsed();
                break;
            case TokenType::JT_STRING:
                _visitor->stringParsed(t1.getValue());
                break;
            case TokenType::JT_NULL:
                _visitor->pointerParsed(t1.getValue());
                break;
            case TokenType::JT_BOOL:
                _visitor->booleanParsed(t1.getValue());
                break;
            case TokenType::JT_NUMBER:
                _visitor->doubleParsed(t1.getValue());
                break;
            case TokenType::JT_INTEGER:
                _visitor->integerParsed(t1.getValue());
                break;
            case TokenType::JT_UNDEFINED:
            case TokenType::JT_COLON:
            case TokenType::JT_COMMA:
            case TokenType::JT_R_BRACE:
            case TokenType::JT_R_BRACKET:
                _visitor->parseError(t1);
                return;
            }

            scn.scan(tok);
            if (tok.getType() == TokenType::JT_NULL)
            {
                _visitor->parseError(tok);
                return;
            }
        }

        _visitor->arrayFinished();
    }
}  // namespace Rt2::Json
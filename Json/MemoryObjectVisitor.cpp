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
#include "MemoryObjectVisitor.h"
#include "ArrayType.h"
#include "BoolType.h"
#include "DoubleType.h"
#include "IntegerType.h"
#include "ObjectType.h"
#include "PointerType.h"
#include "Scanner.h"
#include "StringType.h"
#include "Token.h"
#include "Utils/Stack.h"

namespace Rt2::Json
{
    MemoryObjectVisitor::~MemoryObjectVisitor()
    {
        clear();

        delete _root;
        _root = nullptr;
    }

    void MemoryObjectVisitor::clear()
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

    void MemoryObjectVisitor::parseError(const Token& last)
    {
        Console::writeError("Parse error: ", last.value().c_str());
        clear();
    }

    Type* MemoryObjectVisitor::root()
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

    void MemoryObjectVisitor::arrayCreated()
    {
        _arrStack.push(new ArrayType());
    }

    void MemoryObjectVisitor::objectCreated()
    {
        _objStack.push(new ObjectType());
    }

    void MemoryObjectVisitor::objectFinished()
    {
        if (!_objStack.empty())
        {
            _finishedObjects.push(_objStack.top());
            _objStack.pop();
        }
    }

    void MemoryObjectVisitor::arrayFinished()
    {
        if (!_arrStack.empty())
        {
            _finishedArrays.push(_arrStack.top());
            _arrStack.pop();
        }
    }

    void MemoryObjectVisitor::keyValueParsed(const String&    key,
                                             const TokenType& valueType,
                                             const String&    value)
    {
        if (_objStack.empty())
        {
            Console::writeError("no object on the parse stack\n");
            return;
        }

        ObjectType* top = _objStack.top();

        Type* obj = nullptr;
        switch (valueType)
        {
        case JT_L_BRACE:
            if (!_finishedArrays.empty())
            {
                obj = _finishedArrays.top();
                _finishedArrays.pop();
            }
            break;
        case JT_L_BRACKET:
            if (!_finishedObjects.empty())
            {
                obj = _finishedObjects.top();
                _finishedObjects.pop();
            }
            break;
        case JT_STRING:
            obj = new StringType();
            break;
        case JT_NULL:
            obj = new PointerType();
            break;
        case JT_BOOL:
            obj = new BoolType();
            break;
        case JT_NUMBER:
            obj = new DoubleType();
            break;
        case JT_INTEGER:
            obj = new IntegerType();
            break;
        case JT_UNDEFINED:
        case JT_COLON:
        case JT_COMMA:
        case JT_R_BRACE:
        case JT_R_BRACKET:
            break;
        }

        if (obj != nullptr)
            obj->setValue(value);
        top->insert(key, obj);
    }

    void MemoryObjectVisitor::handleArrayType(Type* obj, const String& value)
    {
        if (obj != nullptr)
        {
            obj->setValue(value);
            auto* arrayObject = _arrStack.top();
            arrayObject->add(obj);
        }
    }

    void MemoryObjectVisitor::objectParsed()
    {
        if (!_arrStack.empty() && !_finishedObjects.empty())
        {
            _arrStack.top()->add(_finishedObjects.top());
            _finishedObjects.pop();
        }
    }

    void MemoryObjectVisitor::arrayParsed()
    {
        if (!_arrStack.empty() && !_finishedArrays.empty())
        {
            _arrStack.top()->add(_finishedArrays.top());
            _finishedArrays.pop();
        }
    }

    void MemoryObjectVisitor::stringParsed(const String& value)
    {
        if (!_arrStack.empty())
            handleArrayType(new ArrayType(), value);
    }

    void MemoryObjectVisitor::integerParsed(const String& value)
    {
        if (!_arrStack.empty())
            handleArrayType(new IntegerType(), value);
    }

    void MemoryObjectVisitor::doubleParsed(const String& value)
    {
        if (!_arrStack.empty())
            handleArrayType(new DoubleType(), value);
    }

    void MemoryObjectVisitor::booleanParsed(const String& value)
    {
        if (!_arrStack.empty())
            handleArrayType(new DoubleType(), value);
    }

    void MemoryObjectVisitor::pointerParsed(const String& value)
    {
        if (!_arrStack.empty())
        {
            handleArrayType(new PointerType(), value);
        }
    }
}  // namespace Rt2::Json
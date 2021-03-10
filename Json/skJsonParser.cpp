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
#include "skJsonParser.h"
#include "Utils/skLogger.h"
#include "Utils/skStack.h"
#include "skJsonArray.h"
#include "skJsonBoolean.h"
#include "skJsonDouble.h"
#include "skJsonInteger.h"
#include "skJsonObject.h"
#include "skJsonPointer.h"
#include "skJsonScanner.h"
#include "skJsonString.h"
#include "skJsonToken.h"
#include "skJsonVisitor.h"

/// <summary>
/// Default visitor
/// Creates type wrappers for parsed json types.
/// </summary>
class skStoreObjectVisitor final : public skJsonVisitor
{
public:
    typedef skStack<skJsonObject*> ObjectStack;
    typedef skStack<skJsonArray*>  ArrayStack;

private:
    skJsonType* m_root;
    ObjectStack m_objStack;
    ArrayStack  m_arrStack;
    ObjectStack m_finishedObjects;
    ArrayStack  m_finishedArrays;

public:
    skStoreObjectVisitor() :
        m_root(nullptr)
    {
    }

    ~skStoreObjectVisitor() override
    {
        clear();

        delete m_root;
        m_root = nullptr;
    }

    void clear()
    {
        while (!m_arrStack.empty())
        {
            delete m_arrStack.top();
            m_arrStack.pop();
        }
        while (!m_objStack.empty())
        {
            delete m_objStack.top();
            m_objStack.pop();
        }
        while (!m_finishedObjects.empty())
        {
            delete m_finishedObjects.top();
            m_finishedObjects.pop();
        }
        while (!m_finishedObjects.empty())
        {
            delete m_finishedObjects.top();
            m_finishedObjects.pop();
        }
    }

    void parseError(const skJsonToken& last) override
    {
        skLogf(LD_ERROR, "Parse error: %s\n", last.getValue().c_str());
        clear();
    }

    skJsonType* getRoot() override
    {
        if (!m_finishedObjects.empty())
        {
            m_root = m_finishedObjects.top();
            m_finishedObjects.pop();
        }
        else if (!m_finishedArrays.empty())
        {
            m_root = m_finishedArrays.top();
            m_finishedArrays.pop();
        }
        return m_root;
    }

    void arrayCreated() override
    {
        m_arrStack.push(new skJsonArray());
    }

    void objectCreated() override
    {
        m_objStack.push(new skJsonObject());
    }

    void objectFinished() override
    {
        if (!m_objStack.empty())
        {
            m_finishedObjects.push(m_objStack.top());
            m_objStack.pop();
        }
    }

    void arrayFinished() override
    {
        if (!m_arrStack.empty())
        {
            m_finishedArrays.push(m_arrStack.top());
            m_arrStack.pop();
        }
    }

    void keyValueParsed(const skString&        key,
                        const skJsonTokenType& valueType,
                        const skString&        value) override
    {
        if (m_objStack.empty())
        {
            skLogf(LD_ERROR, "no json object is on the stack\n");
            return;
        }

        skJsonObject* top = m_objStack.top();

        skJsonType* obj = nullptr;
        switch (valueType)
        {
        case skJsonTokenType::JT_L_BRACE:
            if (!m_finishedArrays.empty())
            {
                obj = m_finishedArrays.top();
                m_finishedArrays.pop();
            }
            break;
        case skJsonTokenType::JT_L_BRACKET:
            if (!m_finishedObjects.empty())
            {
                obj = m_finishedObjects.top();
                m_finishedObjects.pop();
            }
            break;
        case skJsonTokenType::JT_STRING:
            obj = new skJsonString();
            break;
        case skJsonTokenType::JT_NULL:
            obj = new skJsonPointer();
            break;
        case skJsonTokenType::JT_BOOL:
            obj = new skJsonBoolean();
            break;
        case skJsonTokenType::JT_NUMBER:
            obj = new skJsonDouble();
            break;
        case skJsonTokenType::JT_INTEGER:
            obj = new skJsonInteger();
            break;
        case skJsonTokenType::JT_UNDEFINED:
        case skJsonTokenType::JT_COLON:
        case skJsonTokenType::JT_COMMA:
        case skJsonTokenType::JT_R_BRACE:
        case skJsonTokenType::JT_R_BRACKET:
            break;
        }

        if (obj != nullptr)
            obj->setValue(value);
        top->insert(key, obj);
    }

    void handleArrayType(skJsonType* obj, const skString& value)
    {
        if (obj != nullptr)
        {
            obj->setValue(value);
            auto* arrayObject = m_arrStack.top();
            arrayObject->add(obj);
        }
    }

    void objectParsed() override
    {
        if (!m_arrStack.empty() && !m_finishedObjects.empty())
        {
            m_arrStack.top()->add(m_finishedObjects.top());
            m_finishedObjects.pop();
        }
    }

    void arrayParsed() override
    {
        if (!m_arrStack.empty() && !m_finishedArrays.empty())
        {
            m_arrStack.top()->add(m_finishedArrays.top());
            m_finishedArrays.pop();
        }
    }

    void stringParsed(const skString& value) override
    {
        if (!m_arrStack.empty())
            handleArrayType(new skJsonString(), value);
    }

    void integerParsed(const skString& value) override
    {
        if (!m_arrStack.empty())
            handleArrayType(new skJsonInteger(), value);
    }

    void doubleParsed(const skString& value) override
    {
        if (!m_arrStack.empty())
            handleArrayType(new skJsonDouble(), value);
    }

    void booleanParsed(const skString& value) override
    {
        if (!m_arrStack.empty())
            handleArrayType(new skJsonDouble(), value);
    }

    void pointerParsed(const skString& value) override
    {
        if (!m_arrStack.empty())
        {
            handleArrayType(new skJsonPointer(), value);
        }
    }
};

skJsonParser::skJsonParser(skJsonVisitor* visitor) :
    m_visitor(visitor),
    m_owns(visitor == nullptr)
{
    if (m_visitor == nullptr)
        m_visitor = new skStoreObjectVisitor();
}

skJsonParser::~skJsonParser()
{
    if (m_owns)
        delete m_visitor;
}

skJsonType* skJsonParser::parseCommon(skJsonScanner& scanner)
{
    skJsonToken tok;
    scanner.scan(tok);

    skJsonType* root;
    if (tok.getType() == skJsonTokenType::JT_L_BRACKET)
    {
        parseObject(scanner, tok);
        root = m_visitor->getRoot();
    }
    else if (tok.getType() == skJsonTokenType::JT_L_BRACE)
    {
        parseArray(scanner, tok);
        root = m_visitor->getRoot();
    }
    else
        root = nullptr;
    return root;
}

skJsonType* skJsonParser::parse(const skString& path)
{
    skJsonScanner scn;
    scn.open(path);

    if (!scn.isOpen())
    {
        skLogf(LD_ERROR, "failed to open the file '%s'\n", path.c_str());
        return nullptr;
    }
    return parseCommon(scn);
}

skJsonType* skJsonParser::parse(const char* src, SKsize sizeInBytes)
{
    skJsonScanner scn;
    scn.open(src, sizeInBytes);

    if (!scn.isOpen())
    {
        skLogf(LD_ERROR, "failed to open the json memory file \n");
        return nullptr;
    }
    return parseCommon(scn);
}

void skJsonParser::parseObject(skJsonScanner& scn, skJsonToken& tok)
{
    m_visitor->objectCreated();
    skJsonToken t1, t2;

    while (tok.getType() != skJsonTokenType::JT_R_BRACKET)
    {
        scn.scan(t1);

        if (t1.getType() == skJsonTokenType::JT_R_BRACKET)
        {
            break;  // empty
        }

        if (t1.getType() != skJsonTokenType::JT_STRING)
        {
            m_visitor->parseError(t1);
            return;
        }

        scn.scan(t2);
        if (t2.getType() != skJsonTokenType::JT_COLON)
        {
            m_visitor->parseError(t2);
            return;
        }
        scn.scan(t2);

        skJsonTokenType type = t2.getType();
        switch (type)
        {
        case skJsonTokenType::JT_L_BRACE:
            parseArray(scn, t2);
            break;
        case skJsonTokenType::JT_L_BRACKET:
            parseObject(scn, t2);
            break;
        case skJsonTokenType::JT_STRING:
        case skJsonTokenType::JT_NULL:
        case skJsonTokenType::JT_BOOL:
        case skJsonTokenType::JT_NUMBER:
        case skJsonTokenType::JT_INTEGER:
            break;
        case skJsonTokenType::JT_UNDEFINED:
        case skJsonTokenType::JT_COLON:
        case skJsonTokenType::JT_COMMA:
        case skJsonTokenType::JT_R_BRACE:
        case skJsonTokenType::JT_R_BRACKET:
            m_visitor->parseError(t2);
            return;
        }

        m_visitor->keyValueParsed(t1.getValue(), type, t2.getValue());

        scn.scan(tok);
        if (tok.getType() == skJsonTokenType::JT_NULL)
        {
            m_visitor->parseError(tok);
            return;
        }
    }

    m_visitor->objectFinished();
}

void skJsonParser::parseArray(skJsonScanner& scn, skJsonToken& tok)
{
    m_visitor->arrayCreated();
    skJsonToken t1;

    while (tok.getType() != skJsonTokenType::JT_R_BRACE)
    {
        scn.scan(t1);
        if (t1.getType() == skJsonTokenType::JT_R_BRACE)
        {
            break;  // empty
        }

        const skJsonTokenType type = t1.getType();
        switch (type)
        {
        case skJsonTokenType::JT_L_BRACE:
            parseArray(scn, t1);
            m_visitor->arrayParsed();
            break;
        case skJsonTokenType::JT_L_BRACKET:
            parseObject(scn, t1);
            m_visitor->objectParsed();
            break;
        case skJsonTokenType::JT_STRING:
            m_visitor->stringParsed(t1.getValue());
            break;
        case skJsonTokenType::JT_NULL:
            m_visitor->pointerParsed(t1.getValue());
            break;
        case skJsonTokenType::JT_BOOL:
            m_visitor->booleanParsed(t1.getValue());
            break;
        case skJsonTokenType::JT_NUMBER:
            m_visitor->doubleParsed(t1.getValue());
            break;
        case skJsonTokenType::JT_INTEGER:
            m_visitor->integerParsed(t1.getValue());
            break;
        case skJsonTokenType::JT_UNDEFINED:
        case skJsonTokenType::JT_COLON:
        case skJsonTokenType::JT_COMMA:
        case skJsonTokenType::JT_R_BRACE:
        case skJsonTokenType::JT_R_BRACKET:
            m_visitor->parseError(t1);
            return;
        }

        scn.scan(tok);
        if (tok.getType() == skJsonTokenType::JT_NULL)
        {
            m_visitor->parseError(tok);
            return;
        }
    }

    m_visitor->arrayFinished();
}

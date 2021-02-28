#include "TestConfig.h"
#include "gtest/gtest.h"
#include "Json/skJsonArray.h"
#include "Json/skJsonBoolean.h"
#include "Json/skJsonDouble.h"
#include "Json/skJsonInteger.h"
#include "Json/skJsonObject.h"
#include "Json/skJsonParser.h"
#include "Json/skJsonScanner.h"
#include "Json/skJsonString.h"
#include "Json/skJsonToken.h"
#include "Json/skJsonType.h"

#define MakeTestFile(x) TestDirectory x

GTEST_TEST(Test1, Token)
{
    skJsonToken tok;
    tok.setType(skJsonTokenType::JT_UNDEFINED);
    tok.push("A");

    EXPECT_TRUE(tok.getValue().equals("A"));
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_UNDEFINED);
    tok.clear();
    EXPECT_TRUE(tok.getValue().equals(""));
    EXPECT_EQ(tok.getValue().size(), 0);
    EXPECT_LE(tok.getValue().capacity(), 64);

    tok.push('H');
    tok.push('e');
    tok.push('l');
    tok.push('l');
    tok.push('o');
    tok.push(' ');
    tok.push('W');
    tok.push('o');
    tok.push('r');
    tok.push('l');
    tok.push('d');
    tok.push('!');
    EXPECT_TRUE(tok.getValue().equals("Hello World!"));
    EXPECT_EQ(tok.getValue().size(), 12);
    EXPECT_LE(tok.getValue().capacity(), 64);
}

GTEST_TEST(Test1, ScanTest)
{
    skJsonScanner scanner;
    scanner.open(MakeTestFile("test2.json"));
    EXPECT_TRUE(scanner.isOpen());

    skJsonToken tok;
    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_LBRACKET);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue().equals("Hello"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue().equals("World"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue().equals("abc"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_NUMBER);
    EXPECT_TRUE(tok.getValue().equals("-123.456"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue().equals("bool0"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_BOOL);
    EXPECT_TRUE(tok.getValue().equals("true"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue().equals("bool1"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_BOOL);
    EXPECT_TRUE(tok.getValue().equals("false"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue().equals("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_NULL);
    EXPECT_TRUE(tok.getValue().equals("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue().equals("array"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_LBRACE);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue().equals("a"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_INTEGER);
    EXPECT_TRUE(tok.getValue().equals("1"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_BOOL);
    EXPECT_TRUE(tok.getValue().equals("false"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_NULL);
    EXPECT_TRUE(tok.getValue().equals("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_RBRACE);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_RBRACKET);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), skJsonTokenType::JT_NULL);
}

GTEST_TEST(Test1, ParseTest)
{
    skJsonParser parser;

    skJsonType* nobj = parser.parse(MakeTestFile("test2.json"));
    EXPECT_NE(nobj, nullptr);
    EXPECT_EQ(nobj->getType(), skJsonType::Type::OBJECT);

    skJsonObject* obj = (skJsonObject*)nobj;

    EXPECT_TRUE(obj->hasKey("Hello"));

    skJsonType* type = obj->getValue("Hello");
    EXPECT_NE(type, nullptr);
    EXPECT_EQ(type->getType(), skJsonType::Type::STRING);
    EXPECT_TRUE(type->getString().equals("World"));

    EXPECT_TRUE(obj->hasKey("abc"));

    type = obj->getValue("abc");
    EXPECT_NE(type, nullptr);
    EXPECT_EQ(type->getType(), skJsonType::Type::DOUBLE);
    EXPECT_EQ(type->getDouble(), -123.456);

    delete nobj;
}

void test3ValidateObject(skJsonObject* nobj, bool testArray)
{
    EXPECT_TRUE(nobj->hasKey("A"));
    EXPECT_TRUE(nobj->hasKey("B"));
    EXPECT_TRUE(nobj->hasKey("C"));
    EXPECT_TRUE(nobj->hasKey("D"));
    EXPECT_TRUE(nobj->hasKey("X"));
    EXPECT_TRUE(nobj->hasKey("Y"));
    EXPECT_TRUE(nobj->hasKey("Z"));

    if (testArray)
    {
        EXPECT_TRUE(nobj->hasKey("E"));
    }

    const skString& a = nobj->getValue("A")->getString();
    EXPECT_TRUE(a.equals("String"));

    const bool c = nobj->getValue("C")->getBoolean();
    EXPECT_TRUE(c);

    const bool d = nobj->getValue("D")->getBoolean();
    EXPECT_FALSE(d);

    double v;
    v = nobj->getValue("X")->getDouble();
    EXPECT_EQ(v, 1);

    v = nobj->getValue("Y")->getDouble();
    EXPECT_EQ(v, 0);

    v = nobj->getValue("Z")->getDouble();
    EXPECT_EQ(v, 0);

    if (testArray)
    {
        skJsonType* type = nobj->getValue("E");
        EXPECT_TRUE(type->isArray());
        EXPECT_EQ(3, type->asArray()->size());

        for (int i = 0; i < 3; ++i)
        {
            skJsonType* obj = type->asArray()->at(i);
            EXPECT_NE(obj, nullptr);
            EXPECT_TRUE(obj->isObject());
            skJsonObject* nobj = obj->asObject();
            test3ValidateObject(nobj, false);
        }
    }
}

void test3Validate(skJsonArray* arr)
{
    EXPECT_EQ(3, arr->size());

    for (int i = 0; i < 3; ++i)
    {
        skJsonType* obj = arr->at(i);
        EXPECT_NE(obj, nullptr);
        EXPECT_TRUE(obj->isObject());
        skJsonObject* nobj = obj->asObject();
        test3ValidateObject(nobj, true);
    }
}

GTEST_TEST(Test1, ParseTestTest3)
{
    skJsonParser parser;

    skJsonType* nobj = parser.parse(MakeTestFile("test3.json"));

    EXPECT_NE(nobj, nullptr);
    EXPECT_TRUE(nobj->isArray());

    skJsonArray* arr = nobj->asArray();
    test3Validate(arr);
    delete nobj;
}

GTEST_TEST(Test1, Parse4)
{
    skJsonParser parser;
    skJsonType*  nobj = parser.parse(MakeTestFile("test4.json"));
    EXPECT_NE(nobj, nullptr);
    EXPECT_EQ(nobj->getType(), skJsonType::Type::ARRAY);

    skJsonArray* arr = (skJsonArray*)nobj;
    EXPECT_EQ(7, arr->size());
    EXPECT_EQ(0, arr->intAt(0));
    EXPECT_EQ(1, arr->intAt(1));
    EXPECT_EQ(2, arr->intAt(2));
    EXPECT_EQ(3, arr->intAt(3));
    EXPECT_EQ(4, arr->intAt(4));
    EXPECT_EQ(5, arr->intAt(5));
    EXPECT_TRUE(arr->at(6)->getString().equals("Hello"));
    delete nobj;
}

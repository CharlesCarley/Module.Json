#include "Json/ArrayType.h"
#include "Json/ObjectType.h"
#include "Json/Parser.h"
#include "Json/Printer.h"
#include "Json/Scanner.h"
#include "Json/Token.h"
#include "Json/Type.h"
#include "TestConfig.h"
#include "gtest/gtest.h"

#define MakeTestFile(x) TestDirectory x
using namespace Rt2::Json;


GTEST_TEST(Test1, Token)
{
    Rt2::Json::Token tok;
    tok.setType(Rt2::Json::TokenType::JT_UNDEFINED);
    tok.push("A");

    EXPECT_TRUE(tok.getValue()=="A");
    EXPECT_EQ(tok.getType(), Rt2::Json::TokenType::JT_UNDEFINED);
    tok.clear();
    EXPECT_TRUE(tok.getValue().empty());
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
    EXPECT_TRUE(tok.getValue()== ("Hello World!"));
    EXPECT_EQ(tok.getValue().size(), 12);
    EXPECT_LE(tok.getValue().capacity(), 64);
}

GTEST_TEST(Test1, ScanTest)
{
    Scanner scanner;
    scanner.open(MakeTestFile("test2.json"));
    EXPECT_TRUE(scanner.isOpen());

    Token tok;
    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_L_BRACKET);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue()==("Hello"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue()==("World"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue()==("abc"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_NUMBER);
    EXPECT_TRUE(tok.getValue()==("-123.456"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue()==("bool0"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_BOOL);
    EXPECT_TRUE(tok.getValue()==("true"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue()==("bool1"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_BOOL);
    EXPECT_TRUE(tok.getValue()==("false"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue()==("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_NULL);
    EXPECT_TRUE(tok.getValue()==("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue()==("array"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_L_BRACE);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.getValue()==("a"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_INTEGER);
    EXPECT_TRUE(tok.getValue()==("1"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_BOOL);
    EXPECT_TRUE(tok.getValue()==("false"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_NULL);
    EXPECT_TRUE(tok.getValue()==("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_R_BRACE);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_R_BRACKET);

    scanner.scan(tok);
    EXPECT_EQ(tok.getType(), TokenType::JT_NULL);
}

GTEST_TEST(Test1, ParseTest)
{
    Parser parser;

    Type* nObj = parser.parse(MakeTestFile("test2.json"));
    EXPECT_NE(nObj, nullptr);
    EXPECT_EQ(nObj->getType(), Type::Type::OBJECT);

    ObjectType* obj = (ObjectType*)nObj;

    EXPECT_TRUE(obj->hasKey("Hello"));

    Type* type = obj->getValue("Hello");
    EXPECT_NE(type, nullptr);
    EXPECT_EQ(type->getType(), Type::Type::STRING);
    EXPECT_TRUE(type->getString()==("World"));

    EXPECT_TRUE(obj->hasKey("abc"));

    type = obj->getValue("abc");
    EXPECT_NE(type, nullptr);
    EXPECT_EQ(type->getType(), Type::Type::DOUBLE);
    EXPECT_EQ(type->getDouble(), -123.456);
}

void Test3ValidateObject(ObjectType* nObj, bool testArray)
{
    EXPECT_TRUE(nObj->hasKey("A"));
    EXPECT_TRUE(nObj->hasKey("B"));
    EXPECT_TRUE(nObj->hasKey("C"));
    EXPECT_TRUE(nObj->hasKey("D"));
    EXPECT_TRUE(nObj->hasKey("X"));
    EXPECT_TRUE(nObj->hasKey("Y"));
    EXPECT_TRUE(nObj->hasKey("Z"));

    if (testArray)
    {
        EXPECT_TRUE(nObj->hasKey("E"));
    }

    const Rt2::String& a = nObj->getValue("A")->getString();
    EXPECT_TRUE(a==("String"));

    const bool c = nObj->getValue("C")->getBoolean();
    EXPECT_TRUE(c);

    const bool d = nObj->getValue("D")->getBoolean();
    EXPECT_FALSE(d);

    double v;
    v = nObj->getValue("X")->getDouble();
    EXPECT_EQ(v, 1);

    v = nObj->getValue("Y")->getDouble();
    EXPECT_EQ(v, 0);

    v = nObj->getValue("Z")->getDouble();
    EXPECT_EQ(v, 0);

    if (testArray)
    {
        Type* type = nObj->getValue("E");
        EXPECT_TRUE(type->isArray());
        EXPECT_EQ(3, type->asArray()->size());

        for (int i = 0; i < 3; ++i)
        {
            Type* obj = type->asArray()->at(i);
            EXPECT_NE(obj, nullptr);
            EXPECT_TRUE(obj->isObject());

            ObjectType* tObj = obj->asObject();
            Test3ValidateObject(tObj, false);
        }
    }
}

void Test3Validate(ArrayType* arr)
{
    EXPECT_EQ(3, arr->size());

    for (int i = 0; i < 3; ++i)
    {
        Type* obj = arr->at(i);
        EXPECT_NE(obj, nullptr);
        EXPECT_TRUE(obj->isObject());
        ObjectType* tObj = obj->asObject();
        Test3ValidateObject(tObj, true);
    }
}

GTEST_TEST(Test1, ParseTestTest3)
{
    Parser parser;
    Type*  nObj = parser.parse(MakeTestFile("test3.json"));

    EXPECT_NE(nObj, nullptr);
    EXPECT_TRUE(nObj->isArray());

    ArrayType* arr = nObj->asArray();
    Test3Validate(arr);
}

GTEST_TEST(Test1, Parse4)
{
    Parser parser;
    Type*  nObj = parser.parse(MakeTestFile("test4.json"));
    EXPECT_NE(nObj, nullptr);
    EXPECT_EQ(nObj->getType(), Type::Type::ARRAY);

    ArrayType* arr = (ArrayType*)nObj;
    EXPECT_EQ(7, arr->size());
    EXPECT_EQ(0, arr->int16(0));
    EXPECT_EQ(1, arr->int16(1));
    EXPECT_EQ(2, arr->int16(2));
    EXPECT_EQ(3, arr->int16(3));
    EXPECT_EQ(4, arr->int16(4));
    EXPECT_EQ(5, arr->int16(5));
    EXPECT_TRUE(arr->at(6)->getString()==("Hello"));
}

GTEST_TEST(Test2, CreateObjectAndReflect)
{
    ObjectType obj;
    obj.insert("a", 123);

    Rt2::String actual;
    obj.toString(actual);

    const Rt2::String expected1 = R"({"a":123})";
    EXPECT_TRUE(expected1==(actual));

    obj.insert("b", 456);
    obj.insert("c", 789);

    const Rt2::String expected2 = R"({"a":123,"b":456,"c":789})";
    obj.toString(actual);
    EXPECT_TRUE(expected2==(actual));

    Parser parser;
    Type*  type = parser.parse(expected2.c_str(), expected2.size());
    EXPECT_NE(type, nullptr);
    EXPECT_TRUE(type->isObject());

    ObjectType* jObj = type->asObject();
    EXPECT_TRUE(jObj->hasKey("a"));
    EXPECT_TRUE(jObj->hasKey("b"));
    EXPECT_TRUE(jObj->hasKey("c"));

    EXPECT_EQ(jObj->getInt16("c"), 789);
    EXPECT_EQ(jObj->getInt16("b"), 456);
    EXPECT_EQ(jObj->getInt16("a"), 123);
}

GTEST_TEST(Test2, CreateArrayAndReflect)
{
    ArrayType obj;
    obj.add(0);
    obj.add(1);
    obj.add(2);
    obj.add(3);
    obj.add(4);
    obj.add(5);

    Rt2::String actual;
    obj.toString(actual);

    const Rt2::String expected1 = "[0,1,2,3,4,5]";
    EXPECT_TRUE(expected1==(actual));

    obj.add(6);
    obj.add(7);
    obj.add(8);
    obj.add(9);

    const Rt2::String expected2 = "[0,1,2,3,4,5,6,7,8,9]";
    obj.toString(actual);
    EXPECT_TRUE(expected2==(actual));

    Parser parser;
    Type*  type = parser.parse(expected2.c_str(), expected2.size());
    EXPECT_NE(type, nullptr);
    EXPECT_TRUE(type->isArray());

    ArrayType* jObj = type->asArray();
    EXPECT_EQ(0, jObj->int16(0));
    EXPECT_EQ(1, jObj->int16(1));
    EXPECT_EQ(2, jObj->int16(2));
    EXPECT_EQ(3, jObj->int16(3));
    EXPECT_EQ(4, jObj->int16(4));
    EXPECT_EQ(5, jObj->int16(5));
    EXPECT_EQ(6, jObj->int16(6));
    EXPECT_EQ(7, jObj->int16(7));
    EXPECT_EQ(8, jObj->int16(8));
    EXPECT_EQ(9, jObj->int16(9));
}

GTEST_TEST(Test2, PrintFormated)
{
    ObjectType obj;
    obj.insert("a", 123);
    obj.insert("b", 456);
    obj.insert("c", 789);

    Printer print;
    print.writeToStdout(&obj);

    Parser parser;
    Type*  nObj = parser.parse(MakeTestFile("test3.json"));

    print.writeToStdout(nObj);
}

GTEST_TEST(Test2, Test3Reflect)
{
    Parser parser;
    Type*  nObj = parser.parse(MakeTestFile("test3.json"));

    EXPECT_NE(nObj, nullptr);
    EXPECT_TRUE(nObj->isArray());

    // validate initially
    ArrayType* arr = nObj->asArray();
    Test3Validate(arr);

    // validate compactly

    const Rt2::String rp1s = nObj->toString();

    Parser reparse1;
    Type*  rp1 = reparse1.parse(rp1s.c_str(), rp1s.size());

    EXPECT_NE(rp1, nullptr);
    EXPECT_TRUE(rp1->isArray());
    Test3Validate(rp1->asArray());

    // validate formatted;

    Rt2::String      rp2s;
    Printer print;
    print.writeToString(rp2s, nObj);

    Parser reparse2;
    Type*  rp2 = reparse1.parse(rp2s.c_str(), rp2s.size());

    EXPECT_NE(rp2, nullptr);
    EXPECT_TRUE(rp2->isArray());
    Test3Validate(rp2->asArray());
}

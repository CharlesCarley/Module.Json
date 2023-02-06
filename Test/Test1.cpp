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


GTEST_TEST(Token, Token_001)
{
    Token tok;
    tok.setType(JT_UNDEFINED);
    tok.push("A");

    EXPECT_TRUE(tok.value()=="A");
    EXPECT_EQ(tok.type(), Rt2::Json::TokenType::JT_UNDEFINED);
    tok.clear();
    EXPECT_TRUE(tok.value().empty());
    EXPECT_EQ(tok.value().size(), 0);
    EXPECT_LE(tok.value().capacity(), 64);

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
    EXPECT_TRUE(tok.value()== ("Hello World!"));
    EXPECT_EQ(tok.value().size(), 12);
    EXPECT_LE(tok.value().capacity(), 64);
}

GTEST_TEST(Scanner, Scan_001)
{
    Scanner scanner;
    scanner.open(MakeTestFile("test2.json"));
    EXPECT_TRUE(scanner.isOpen());

    Token tok;
    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_L_BRACKET);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.value()==("Hello"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.value()==("World"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.value()==("abc"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_NUMBER);
    EXPECT_TRUE(tok.value()==("-123.456"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.value()==("bool0"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_BOOL);
    EXPECT_TRUE(tok.value()==("true"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.value()==("bool1"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_BOOL);
    EXPECT_TRUE(tok.value()==("false"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.value()==("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_NULL);
    EXPECT_TRUE(tok.value()==("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.value()==("array"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COLON);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_L_BRACE);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_STRING);
    EXPECT_TRUE(tok.value()==("a"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_INTEGER);
    EXPECT_TRUE(tok.value()==("1"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_BOOL);
    EXPECT_TRUE(tok.value()==("false"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_COMMA);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_NULL);
    EXPECT_TRUE(tok.value()==("null"));

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_R_BRACE);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_R_BRACKET);

    scanner.scan(tok);
    EXPECT_EQ(tok.type(), TokenType::JT_NULL);
}

GTEST_TEST(Parser, Parse_001)
{
    Parser parser;

    Type* nObj = parser.parse(MakeTestFile("test2.json"));
    EXPECT_NE(nObj, nullptr);
    EXPECT_EQ(nObj->type(), Type::Type::OBJECT);

    ObjectType* obj = (ObjectType*)nObj;

    EXPECT_TRUE(obj->hasKey("Hello"));

    Type* type = obj->find("Hello");
    EXPECT_NE(type, nullptr);
    EXPECT_EQ(type->type(), Type::Type::STRING);
    EXPECT_TRUE(type->string()==("World"));

    EXPECT_TRUE(obj->hasKey("abc"));

    type = obj->find("abc");
    EXPECT_NE(type, nullptr);
    EXPECT_EQ(type->type(), Type::Type::DOUBLE);
    EXPECT_EQ(type->r64(), -123.456);
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

    const Rt2::String& a = nObj->find("A")->string();
    EXPECT_TRUE(a==("String"));

    const bool c = nObj->find("C")->boolean();
    EXPECT_TRUE(c);

    const bool d = nObj->find("D")->boolean();
    EXPECT_FALSE(d);

    double v;
    v = nObj->find("X")->r64();
    EXPECT_EQ(v, 1);

    v = nObj->find("Y")->r64();
    EXPECT_EQ(v, 0);

    v = nObj->find("Z")->r64();
    EXPECT_EQ(v, 0);

    if (testArray)
    {
        Type* type = nObj->find("E");
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

GTEST_TEST(Parser, Parse_002)
{
    Parser parser;
    Type*  nObj = parser.parse(MakeTestFile("test3.json"));

    EXPECT_NE(nObj, nullptr);
    EXPECT_TRUE(nObj->isArray());

    ArrayType* arr = nObj->asArray();
    Test3Validate(arr);
}

GTEST_TEST(Parser, Parse_003)
{
    Parser parser;
    Type*  nObj = parser.parse(MakeTestFile("test4.json"));
    EXPECT_NE(nObj, nullptr);
    EXPECT_EQ(nObj->type(), Type::Type::ARRAY);

    ArrayType* arr = (ArrayType*)nObj;
    EXPECT_EQ(7, arr->size());
    EXPECT_EQ(0, arr->i16(0));
    EXPECT_EQ(1, arr->i16(1));
    EXPECT_EQ(2, arr->i16(2));
    EXPECT_EQ(3, arr->i16(3));
    EXPECT_EQ(4, arr->i16(4));
    EXPECT_EQ(5, arr->i16(5));
    EXPECT_TRUE(arr->at(6)->string()==("Hello"));
}

GTEST_TEST(Parser, ToString_001)
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

    EXPECT_EQ(jObj->i16("c"), 789);
    EXPECT_EQ(jObj->i16("b"), 456);
    EXPECT_EQ(jObj->i16("a"), 123);
}

GTEST_TEST(Parser, ToString_002)
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
    EXPECT_EQ(0, jObj->i16(0));
    EXPECT_EQ(1, jObj->i16(1));
    EXPECT_EQ(2, jObj->i16(2));
    EXPECT_EQ(3, jObj->i16(3));
    EXPECT_EQ(4, jObj->i16(4));
    EXPECT_EQ(5, jObj->i16(5));
    EXPECT_EQ(6, jObj->i16(6));
    EXPECT_EQ(7, jObj->i16(7));
    EXPECT_EQ(8, jObj->i16(8));
    EXPECT_EQ(9, jObj->i16(9));
}

GTEST_TEST(Printer, Formatted_001)
{
    ObjectType obj;
    obj.insert("a", 123);
    obj.insert("b", 456);
    obj.insert("c", 789);

    const Printer print;
    print.writeToStdout(&obj);

    Parser parser;
    Type*  nObj = parser.parse(MakeTestFile("test3.json"));

    print.writeToStdout(nObj);
}

GTEST_TEST(Parser, Parse_Reflection)
{
    Parser parser;
    Type*  nObj = parser.parse(MakeTestFile("test3.json"));

    EXPECT_NE(nObj, nullptr);
    EXPECT_TRUE(nObj->isArray());

    // validate initially
    ArrayType* arr = nObj->asArray();
    Test3Validate(arr);

    // validate compactly

    const Rt2::String rp1S = nObj->toString();

    Parser reparse1;
    Type*  rp1 = reparse1.parse(rp1S.c_str(), rp1S.size());

    EXPECT_NE(rp1, nullptr);
    EXPECT_TRUE(rp1->isArray());
    Test3Validate(rp1->asArray());

    // validate formatted;

    Rt2::String   rp2S;
    const Printer print;
    print.writeToString(rp2S, nObj);

    Parser reparse2;
    Type*  rp2 = reparse1.parse(rp2S.c_str(), rp2S.size());

    EXPECT_NE(rp2, nullptr);
    EXPECT_TRUE(rp2->isArray());
    Test3Validate(rp2->asArray());
}



#include "Json/skJsonArray.h"
#include "Json/skJsonObject.h"
#include "Json/skJsonParser.h"
#include "Json/skJsonPrinter.h"
#include "Json/skJsonScanner.h"
#include "Json/skJsonToken.h"
#include "Json/skJsonType.h"
#include "TestConfig.h"
#include "gtest/gtest.h"

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

    skJsonType* nObj = parser.parse(MakeTestFile("test2.json"));
    EXPECT_NE(nObj, nullptr);
    EXPECT_EQ(nObj->getType(), skJsonType::Type::OBJECT);

    skJsonObject* obj = (skJsonObject*)nObj;

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
}

void Test3ValidateObject(skJsonObject* nObj, bool testArray)
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

    const skString& a = nObj->getValue("A")->getString();
    EXPECT_TRUE(a.equals("String"));

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
        skJsonType* type = nObj->getValue("E");
        EXPECT_TRUE(type->isArray());
        EXPECT_EQ(3, type->asArray()->size());

        for (int i = 0; i < 3; ++i)
        {
            skJsonType* obj = type->asArray()->at(i);
            EXPECT_NE(obj, nullptr);
            EXPECT_TRUE(obj->isObject());

            skJsonObject* tObj = obj->asObject();
            Test3ValidateObject(tObj, false);
        }
    }
}

void Test3Validate(skJsonArray* arr)
{
    EXPECT_EQ(3, arr->size());

    for (int i = 0; i < 3; ++i)
    {
        skJsonType* obj = arr->at(i);
        EXPECT_NE(obj, nullptr);
        EXPECT_TRUE(obj->isObject());
        skJsonObject* tObj = obj->asObject();
        Test3ValidateObject(tObj, true);
    }
}

GTEST_TEST(Test1, ParseTestTest3)
{
    skJsonParser parser;
    skJsonType*  nObj = parser.parse(MakeTestFile("test3.json"));

    EXPECT_NE(nObj, nullptr);
    EXPECT_TRUE(nObj->isArray());

    skJsonArray* arr = nObj->asArray();
    Test3Validate(arr);
}

GTEST_TEST(Test1, Parse4)
{
    skJsonParser parser;
    skJsonType*  nObj = parser.parse(MakeTestFile("test4.json"));
    EXPECT_NE(nObj, nullptr);
    EXPECT_EQ(nObj->getType(), skJsonType::Type::ARRAY);

    skJsonArray* arr = (skJsonArray*)nObj;
    EXPECT_EQ(7, arr->size());
    EXPECT_EQ(0, arr->int16(0));
    EXPECT_EQ(1, arr->int16(1));
    EXPECT_EQ(2, arr->int16(2));
    EXPECT_EQ(3, arr->int16(3));
    EXPECT_EQ(4, arr->int16(4));
    EXPECT_EQ(5, arr->int16(5));
    EXPECT_TRUE(arr->at(6)->getString().equals("Hello"));
}

GTEST_TEST(Test2, CreateObjectAndReflect)
{
    skJsonObject obj;
    obj.insert("a", 123);

    skString actual;
    obj.toString(actual);

    const skString expected1 = R"({"a":123})";
    EXPECT_TRUE(expected1.equals(actual));

    obj.insert("b", 456);
    obj.insert("c", 789);

    const skString expected2 = R"({"a":123,"b":456,"c":789})";
    obj.toString(actual);
    EXPECT_TRUE(expected2.equals(actual));

    skJsonParser parser;
    skJsonType*  type = parser.parse(expected2.c_str(), expected2.size());
    EXPECT_NE(type, nullptr);
    EXPECT_TRUE(type->isObject());

    skJsonObject* jObj = type->asObject();
    EXPECT_TRUE(jObj->hasKey("a"));
    EXPECT_TRUE(jObj->hasKey("b"));
    EXPECT_TRUE(jObj->hasKey("c"));

    EXPECT_EQ(jObj->getInt16("c"), 789);
    EXPECT_EQ(jObj->getInt16("b"), 456);
    EXPECT_EQ(jObj->getInt16("a"), 123);
}

GTEST_TEST(Test2, CreateArrayAndReflect)
{
    skJsonArray obj;
    obj.add(0);
    obj.add(1);
    obj.add(2);
    obj.add(3);
    obj.add(4);
    obj.add(5);

    skString actual;
    obj.toString(actual);

    const skString expected1 = "[0,1,2,3,4,5]";
    EXPECT_TRUE(expected1.equals(actual));

    obj.add(6);
    obj.add(7);
    obj.add(8);
    obj.add(9);

    const skString expected2 = "[0,1,2,3,4,5,6,7,8,9]";
    obj.toString(actual);
    EXPECT_TRUE(expected2.equals(actual));

    skJsonParser parser;
    skJsonType*  type = parser.parse(expected2.c_str(), expected2.size());
    EXPECT_NE(type, nullptr);
    EXPECT_TRUE(type->isArray());

    skJsonArray* jObj = type->asArray();
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
    skJsonObject obj;
    obj.insert("a", 123);
    obj.insert("b", 456);
    obj.insert("c", 789);

    skJsonPrinter print;
    print.writeToStdout(&obj);

    skJsonParser parser;
    skJsonType*  nObj = parser.parse(MakeTestFile("test3.json"));

    print.writeToStdout(nObj);
}



GTEST_TEST(Test2, Test3Reflect)
{
    skJsonParser parser;
    skJsonType*  nObj = parser.parse(MakeTestFile("test3.json"));

    EXPECT_NE(nObj, nullptr);
    EXPECT_TRUE(nObj->isArray());

    // validate initially
    skJsonArray* arr = nObj->asArray();
    Test3Validate(arr);

    // validate compactly

    const skString rp1s = nObj->toString();

    skJsonParser reparse1;
    skJsonType*  rp1 = reparse1.parse(rp1s.c_str(), rp1s.size());

    EXPECT_NE(rp1, nullptr);
    EXPECT_TRUE(rp1->isArray());
    Test3Validate(rp1->asArray());


    // validate formatted;

    skString rp2s;
    skJsonPrinter print;
    print.writeToString(rp2s, nObj);


    skJsonParser reparse2;
    skJsonType*  rp2 = reparse1.parse(rp2s.c_str(), rp2s.size());

    EXPECT_NE(rp2, nullptr);
    EXPECT_TRUE(rp2->isArray());
    Test3Validate(rp2->asArray());
}
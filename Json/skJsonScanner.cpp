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
#include "skJsonScanner.h"
#include <memory.h>
#include <cstdio>
#include "Utils/skFileStream.h"
#include "Utils/skMemoryUtils.h"

skJsonScanner::skJsonScanner() :
    m_data(nullptr),
    m_len(SK_NPOS),
    m_pos(SK_NPOS)
{
}

skJsonScanner::~skJsonScanner()
{
    delete[] m_data;
    m_data = nullptr;
}

void skJsonScanner::open(const skString& path)
{
    const skFileStream fs = skFileStream(path.c_str(), skStream::READ);
    if (fs.isOpen())
    {
        const SKsize len = fs.size();
        if (len > 0 && len < SK_NPOS16)
        {
            m_len = len;
            m_pos = 0;
            delete[] m_data;
            m_data = nullptr;

            m_data = new SKint8[m_len + 1];
            fs.read(m_data, m_len);
            m_data[m_len] = 0;
        }
    }
}

void skJsonScanner::open(const char* mem, const SKsize len)
{
    if (mem && len > 0 && len < SK_NPOS16)
    {
        m_len = len;
        m_pos = 0;

        delete[] m_data;
        m_data = nullptr;

        m_data = new SKint8[m_len + 1];
        skMemcpy(m_data, mem, len);
        m_data[m_len] = 0;
    }
}

void skJsonScanner::scan(skJsonToken& tok)
{
    tok.setType(skJsonTokenType::JT_NULL);
    tok.clear();

    if (!m_data)
        return;

    while (m_pos < m_len)
    {
        char ch = m_data[m_pos];
        ++m_pos;

        switch (ch)
        {
        case '/':
        {
            ch = m_data[m_pos];
            if (ch == '/')
            {
                while (ch != '\n' && ch != '\r' && ch != 0)
                {
                    ++m_pos;
                    ch = m_data[m_pos];
                }
                if (ch == 0)
                {
                    m_pos = SK_NPOS;
                    return;
                }
            }
            else
            {
                m_pos = SK_NPOS;
                return;
            }
            break;
        }

        case '[':
            tok.setType(skJsonTokenType::JT_L_BRACE);
            return;
        case ']':
            tok.setType(skJsonTokenType::JT_R_BRACE);
            return;
        case ',':
            tok.setType(skJsonTokenType::JT_COMMA);
            return;
        case ':':
            tok.setType(skJsonTokenType::JT_COLON);
            return;
        case '{':
            tok.setType(skJsonTokenType::JT_L_BRACKET);
            return;
        case '}':
            tok.setType(skJsonTokenType::JT_R_BRACKET);
            return;
        case '\"':
        {
            while (m_data[m_pos] != '\"' && m_data[m_pos] != 0)
                tok.push(m_data[m_pos++]);

            if (m_data[m_pos] == '\"')
            {
                ++m_pos;
                tok.setType(skJsonTokenType::JT_STRING);
                return;
            }
            m_pos = SK_NPOS;
            return;
        }
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            bool hasDot = false;
            tok.push(ch);
            for (; m_pos < m_len; ++m_pos)
            {
                ch = m_data[m_pos];
                if (ch == '.')
                    hasDot = true;
                if (isDigitSet(ch) && ch != 0)
                    tok.push(ch);
                else
                    break;
            }

            if (ch != 0)
            {
                tok.setType(hasDot ? skJsonTokenType::JT_NUMBER
                                   : skJsonTokenType::JT_INTEGER);
                return;
            }
            m_pos = SK_NPOS;
            return;
        }
        case 't':
        case 'f':
        case 'n':
        {
            if (skChar::equalsn((const char*)&m_data[m_pos - 1], "true", 4) == 0)
            {
                m_pos += 3;
                tok.setType(skJsonTokenType::JT_BOOL);
                tok.push("true");
                return;
            }
            if (skChar::equalsn((const char*)&m_data[m_pos - 1], "false", 5) == 0)
            {
                m_pos += 4;
                tok.setType(skJsonTokenType::JT_BOOL);
                tok.push("false");
                return;
            }
            if (skChar::equalsn((const char*)&m_data[m_pos - 1], "null", 4) == 0)
            {
                m_pos += 3;
                tok.setType(skJsonTokenType::JT_NULL);
                tok.push("null");
                return;
            }
            m_pos = SK_NPOS;
            return;
        }
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            break;
        default:
            m_pos = SK_NPOS;
            return;
        }
    }
}

bool skJsonScanner::isDigitSet(const char ch)
{
    return ch >= '0' && ch <= '9' || ch == '-' || ch == '.';
}

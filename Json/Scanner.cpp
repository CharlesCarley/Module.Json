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
#include "Scanner.h"
#include "Utils/Char.h"

namespace Rt2::Json
{
    Scanner::Scanner() :
        _data(nullptr),
        _len(Npos),
        _pos(Npos)
    {
    }

    Scanner::~Scanner()
    {
        delete[] _data;
        _data = nullptr;
    }

    void Scanner::open(const String& path)
    {
        if (InputFileStream fs =
                InputFileStream(path.c_str(),
                                std::ios::ate | std::ios::binary); fs.is_open())
        {
            if (const std::streamsize len = fs.tellg();
                len > 0 && len < Npos16)
            {
                fs.seekg(0, std::ios::beg);
                _len = len;
                _pos = 0;
                delete[] _data;
                _data = nullptr;

                _data = new char[_len + 1];
                fs.read(_data, _len);
                _data[_len] = 0;
            }
        }
    }

    void Scanner::open(const char* mem, const size_t len)
    {
        if (mem && len > 0 && len < Npos16)
        {
            _len = len;
            _pos = 0;

            delete[] _data;
            _data = nullptr;

            _data = new char[_len + 1];
            memcpy(_data, mem, len);
            _data[_len] = 0;
        }
    }

    void Scanner::scan(Token& tok)
    {
        tok.clear();

        if (!_data)
            return;

        while (_pos < _len)
        {
            char ch = _data[_pos];
            ++_pos;

            switch (ch)
            {
            case '/':
            {
                ch = _data[_pos];
                if (ch == '/')
                {
                    while (ch != '\n' && ch != '\r' && ch != 0)
                    {
                        ++_pos;
                        ch = _data[_pos];
                    }
                    if (ch == 0)
                    {
                        _pos = Npos;
                        return;
                    }
                }
                else
                {
                    _pos = Npos;
                    return;
                }
                break;
            }

            case '[':
                tok.setType(JT_L_BRACE);
                return;
            case ']':
                tok.setType(JT_R_BRACE);
                return;
            case ',':
                tok.setType(JT_COMMA);
                return;
            case ':':
                tok.setType(JT_COLON);
                return;
            case '{':
                tok.setType(JT_L_BRACKET);
                return;
            case '}':
                tok.setType(JT_R_BRACKET);
                return;
            case '\"':
            {
                while (_data[_pos] != '\"' && _data[_pos] != 0)
                    tok.push(_data[_pos++]);

                if (_data[_pos] == '\"')
                {
                    ++_pos;
                    tok.setType(JT_STRING);
                    return;
                }
                _pos = Npos;
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
                for (; _pos < _len; ++_pos)
                {
                    ch = _data[_pos];
                    if (ch == '.')
                        hasDot = true;
                    if (isDigitSet(ch) && ch != 0)
                        tok.push(ch);
                    else
                        break;
                }

                if (ch != 0)
                {
                    tok.setType(hasDot ? JT_NUMBER
                                       : JT_INTEGER);
                    return;
                }
                _pos = Npos;
                return;
            }
            case 't':
            case 'f':
            case 'n':
            {
                if (Char::equals((const char*)&_data[_pos - 1], "true", 4))
                {
                    _pos += 3;
                    tok.setType(JT_BOOL);
                    tok.push("true");
                    return;
                }

                if (Char::equals((const char*)&_data[_pos - 1], "false", 5) )
                {
                    _pos += 4;
                    tok.setType(JT_BOOL);
                    tok.push("false");
                    return;
                }

                if (Char::equals((const char*)&_data[_pos - 1], "null", 4))
                {
                    _pos += 3;
                    tok.setType(JT_NULL);
                    tok.push("null");
                    return;
                }
                _pos = Npos;
                return;
            }
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break;
            default:
                _pos = Npos;
                return;
            }
        }
    }

    bool Scanner::isDigitSet(const char ch)
    {
        return ch >= '0' && ch <= '9' || ch == '-' || ch == '.';
    }

}  // namespace Rt2::Json
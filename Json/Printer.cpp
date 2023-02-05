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

#include "Printer.h"
#include <cstdio>
#include "Type.h"
#include "ArrayType.h"
#include "ObjectType.h"

namespace Rt2::Json
{
    class PrinterPrivate
    {
    private:
        StringBuilder _buffer;
        int           _depth;

    public:
        PrinterPrivate() :
            _depth(0)
        {
            _buffer.setStrategy(ALLOC_N_BYTE_BLOCK, 128);
        }

        void writeSpace()
        {
            const char WS[] = {' ', ' ', ' ', ' ', 0};
            for (int i = 0; i < _depth; ++i)
                _buffer.write(WS);
        }

        void writeObject(ObjectType* root)
        {
            _buffer.write('{');
            _buffer.write('\n');

            _depth++;
            bool first = true;

            ObjectType::Dictionary& dict = root->dictionary();

            for (const auto& it : dict)
            {
                if (!first)
                {
                    _buffer.write(',');
                    _buffer.write('\n');
                }
                else
                    first = false;

                writeSpace();
                _buffer.write('"');
                _buffer.write(it.first);
                _buffer.write('"');
                _buffer.write(':');
                _buffer.write(' ');

                if (it.second->isArray())
                {
                    // handle array
                    writeArray(it.second->asArray());
                }
                else if (it.second->isObject())
                {
                    // handle object
                    ++_depth;
                    writeObject(it.second->asObject());
                    --_depth;
                }
                else
                    it.second->toString(_buffer);
            }
            _buffer.write('\n');
            if (_depth-- > 0)
                writeSpace();
            _buffer.write('}');
        }

        void writeArray(ArrayType* array)
        {
            _buffer.write('[');

            for (U32 i = 0; i < array->size(); ++i)
            {
                Type*      idx   = array->at(i);
                const bool split = idx->isObject() || idx->isArray();

                if (i > 0)
                {
                    _buffer.write(',');
                    if (split || i % 20 == 19)
                    {
                        _buffer.write('\n');
                        writeSpace();
                    }
                }
                if (idx->isObject())
                    writeObject(idx->asObject());
                else if (idx->isArray())
                    writeArray(idx->asArray());
                else
                    idx->toString(_buffer);
            }
            _buffer.write(']');
        }

        void write(Type* obj)
        {
            if (obj)
            {
                if (obj->isObject())
                    writeObject(obj->asObject());
                else if (obj->isArray())
                    writeArray(obj->asArray());
            }
        }

        void getBuffer(String& dest) const
        {
            _buffer.toString(dest);
        }
    };

    Printer::Printer()
    {
        _private = new PrinterPrivate();
    }

    Printer::~Printer()
    {
        delete _private;
    }

    void Printer::writeToFile(Type* obj, const String& path) const
    {
        OutputFileStream ofs;
        ofs.open(path);

        if (ofs.is_open())
        {
            _private->write(obj);

            String buffer;
            _private->getBuffer(buffer);
            ofs.write(buffer.c_str(), buffer.size());
        }
        else
            printf("Failed to open '%s'\n ", path.c_str());
    }

    void Printer::writeToStdout(Type* obj) const
    {
        _private->write(obj);

        String buffer;
        _private->getBuffer(buffer);
        if (!buffer.empty())
            puts(buffer.c_str());
    }

    void Printer::writeToString(String& dest, Type* obj) const
    {
        _private->write(obj);
        _private->getBuffer(dest);
    }
}  // namespace Rt2::Json

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

#include "skJsonPrinter.h"
#include <cstdio>
#include "Utils/skFileStream.h"
#include "Utils/skMemoryStream.h"
#include "Utils/skStringBuilder.h"
#include "skJsonArray.h"
#include "skJsonObject.h"
#include "skJsonType.h"

class skJsonPrinterPrivate
{
private:
    skStringBuilder m_buffer;
    int             m_depth;

public:
    skJsonPrinterPrivate() :
        m_depth(0)
    {
        m_buffer.setStrategy(ALLOC_N_BYTE_BLOCK, 128);
    }

    void writeSpace()
    {
        const char WS[] = {' ', ' ', ' ', ' ', 0};
        for (int i = 0; i < m_depth; ++i)
            m_buffer.write(WS, 4);
    }

    void writeObject(skJsonObject* root)
    {
        using jIterator = skJsonObject::Dictionary::Iterator;

        m_buffer.write('{');
        m_buffer.write('\n');

        m_depth++;
        bool first = true;

        skString  tStr;
        jIterator it = root->iterator();
        while (it.hasMoreElements())
        {
            jIterator::ReferenceType kv = it.getNext();
            if (!first)
            {
                m_buffer.write(',');
                m_buffer.write('\n');
            }
            else
                first = false;

            writeSpace();
            m_buffer.write('"');
            m_buffer.write(kv.first);
            m_buffer.write('"');
            m_buffer.write(':');
            m_buffer.write(' ');

            if (kv.second->isArray())
            {
                // handle array
                writeArray(kv.second->asArray());
            }
            else if (kv.second->isObject())
            {
                // handle object
                ++m_depth;
                writeObject(kv.second->asObject());
                --m_depth;
            }
            else
                kv.second->toString(m_buffer);
        }
        m_buffer.write('\n');
        if (m_depth-- > 0)
            writeSpace();
        m_buffer.write('}');
    }

    void writeArray(skJsonArray* array)
    {
        m_buffer.write('[');

        for (SKuint32 i = 0; i < array->size(); ++i)
        {
            skJsonType* idx   = array->at(i);
            const bool  split = idx->isObject() || idx->isArray();

            if (i > 0)
            {
                m_buffer.write(',');
                if (split || i % 20 == 19)
                {
                    m_buffer.write('\n');
                    writeSpace();
                }
            }

            if (idx->isObject())
            {
                writeObject(idx->asObject());
            }
            else if (idx->isArray())
                writeArray(idx->asArray());
            else
                idx->toString(m_buffer);
        }
        m_buffer.write(']');
    }

    void write(skJsonType* obj)
    {
        if (obj)
        {
            if (obj->isObject())
                writeObject(obj->asObject());
            else if (obj->isArray())
                writeArray(obj->asArray());
        }
    }

    void getBuffer(skString& dest) const
    {
        m_buffer.toString(dest);
    }
};

skJsonPrinter::skJsonPrinter()
{
    m_private = new skJsonPrinterPrivate();
}

skJsonPrinter::~skJsonPrinter()
{
    delete m_private;
}

void skJsonPrinter::writeToFile(skJsonType* obj, const skString& path) const
{
    m_private->write(obj);

    skFileStream fs;
    fs.open(path.c_str(), skStream::WRITE);

    if (fs.isOpen())
    {
        skString buffer;
        m_private->getBuffer(buffer);
        fs.write(buffer.c_str(), buffer.size());
    }
    else
    {
        printf("Failed to open '%s'\n ", path.c_str());
    }
}

void skJsonPrinter::writeToStdout(skJsonType* obj) const
{
    m_private->write(obj);
    skString buffer;
    m_private->getBuffer(buffer);
    puts(buffer.c_str());
}

void skJsonPrinter::writeToString(skString& dest, skJsonType* obj) const
{
    m_private->write(obj);
    m_private->getBuffer(dest);
}

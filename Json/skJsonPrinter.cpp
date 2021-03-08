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
#include "skJsonObject.h"
#include "skJsonType.h"

class skJsonPrinterPrivate
{
private:
    skString m_buffer;
    int      m_depth;

public:
    skJsonPrinterPrivate() :
        m_depth(0)
    {
        m_buffer.reserve(2048);  // needs an expansion strategy
    }

    void writeSpace()
    {
        const char WS[] = {' ', ' ', ' ', ' ', 0};
        for (int i = 0; i < m_depth; ++i)
            m_buffer.append(WS, 4);
    }

    void writeObject(skJsonObject* root)
    {
        using jIterator = skJsonObject::Dictionary::Iterator;

        if (m_depth++ > 0)
            writeSpace();

        m_buffer.append('{');
        m_buffer.append('\n');

        bool first = true;

        skString  tStr;
        jIterator it = root->iterator();
        while (it.hasMoreElements())
        {
            jIterator::ReferenceType kv = it.getNext();
            if (!first)
            {
                m_buffer.append(',');
                m_buffer.append('\n');
            }
            else
                first = false;

            writeSpace();

            m_buffer.append('"');
            m_buffer.append(kv.first);
            m_buffer.append('"');
            m_buffer.append(':');
            m_buffer.append(' ');

            if (kv.second->isArray())
            {
                // handle array
            }
            else if (kv.second->isObject())
            {
                // handle object
                ++m_depth;
                writeObject(kv.second->asObject());
            }
            else
            {
                kv.second->toString(tStr);
                m_buffer.append(tStr);
            }
        }
        m_buffer.append('\n');

        if (--m_depth > 0)
            writeSpace();
        m_buffer.append('}');
    }

    void writeArray(skJsonArray* array)
    {
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

    const skString& getBuffer() const
    {
        return m_buffer;
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
        const skString& buffer = m_private->getBuffer();
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
    puts(m_private->getBuffer().c_str());
}

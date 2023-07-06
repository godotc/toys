#pragma once

#include "../pyvm.h"

#include "fmt/core.h"
#include <array>
#include <cstdio>
#include <fstream>
#include <ios>
#include <log.h>


PYVM_TOP_LEVEL_NAMESPACE_BEGIN


inline const int BUFFER_LEN = 256;


class BufferInputStream
{
  public:
    BufferInputStream(char const *filepath)
    {
        ifs.open(filepath, std::ios_base::in);
        if (!ifs.is_open() || ifs.fail()) {
            LOG_ERROR("Failed to open {}", filepath);
            return;
        }

        ifs.read(m_Buffer, BUFFER_LEN);

        LOG_DEBUG("The first data that read:");
        for (int i = 0; i < BUFFER_LEN; ++i) {
            printf("%x", m_Buffer[i]);
        }
        printf("\n");
        for (int i = 0; i < BUFFER_LEN; ++i) {
            printf("%c", m_Buffer[i]);
        }
        printf("\n");

        m_Index = 0;
    }

    ~BufferInputStream()
    {
        ifs.close();
    }

    char read()
    {
        // LOG_DEBUG("{}", m_Buffer.data());
        if (m_Index < BUFFER_LEN)
            return m_Buffer[m_Index++];
        else {
            m_Index = 0;
            ifs.read(m_Buffer, BUFFER_LEN);
            return m_Buffer[m_Index++];
        }
    }

    int read_int()
    {
        int b1 = read() & 0xff;
        int b2 = read() & 0xff;
        int b3 = read() & 0xff;
        int b4 = read() & 0xff;
        // LOG_DEBUG("{}|{}|{}|{}", b1, b2, b3, b4);

        return b4 << 24 | b3 << 16 | b2 << 8 | b1;
    }

    void unread()
    {
        --m_Index;
    }

  private:
    std::ifstream ifs;
    char          m_Buffer[BUFFER_LEN];
    unsigned int  m_Index;
};

PYVM_TOP_LEVEL_NAMESPACE_END

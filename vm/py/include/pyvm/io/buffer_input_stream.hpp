#pragma once

#include "fmt/core.h"
#include <array>
#include <fstream>
#include <ios>
#include <log.h>

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

        ifs.get(m_Buffer.begin()._Unwrapped(), m_Buffer.size());
        m_Index = 0;
    }

    ~BufferInputStream()
    {
        ifs.close();
    }

    char Read()
    {
        // LOG_DEBUG("{}", m_Buffer.begin()._Unwrapped());
        if (m_Index < m_Buffer.size())
            return m_Buffer[m_Index++];
        else {
            m_Index = 0;
            ifs.get(m_Buffer.begin()._Unwrapped(), m_Buffer.size());
            return m_Buffer[m_Index++];
        }
    }

    int ReadInt()
    {
        int b1 = Read() & 0xff;
        int b2 = Read() & 0xff;
        int b3 = Read() & 0xff;
        int b4 = Read() & 0xff;
        // LOG_DEBUG("{}|{}|{}|{}", b1, b2, b3, b4);

        return b4 << 24 | b3 << 16 | b2 << 8 | b1;
    }

    void Unread()
    {
        --m_Index;
    }

  private:
    std::ifstream                ifs;
    std::array<char, BUFFER_LEN> m_Buffer;
    unsigned int                 m_Index;
};

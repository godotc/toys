#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace toplevel_namespace {

class FrameBuffer
{

  public:
    FrameBuffer(int w, int h)
    {
        m_Width = w;
        m_Hight = h;
        m_Buffer.resize(w * h, ' ');
    }


    void SetPixel(glm::vec2 pt, char v)
    {
        SetPixel(pt.x, pt.y, v);
    }

    void SetPixel(int x, int y, char v)
    {
        if (x >= 0 && x < m_Width && y >= 0 && y < m_Hight) {
            auto pos      = m_Width * y + x;
            m_Buffer[pos] = v;
        }
    }

    void
    CleanBuffer()
    {
        for (char &i : m_Buffer)
            i = ' ';
    }

    void Display()
    {
        // move cursor to top-left
        std::cout << "\033[H";
        for (int i = 0; i < m_Hight; ++i) {
            for (int j = 0; j < m_Width; ++j) {
                std::cout /*<< "\033[31m"*/ << m_Buffer[m_Width * i + j];
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }


  private:
    int m_Width, m_Hight;

    std::string m_Buffer;
};
}; // namespace toplevel_namespace

#pragma once

#include "../object.hpp"
#include <cstring>
#include <string.h>
#include <vcruntime_string.h>


class PyInteger : public PyObject
{
  public:
    PyInteger(int v)
    {
        m_Value = v;
    }

    int value() { return m_Value; }

  private:
    int m_Value;
};

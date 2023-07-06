#pragma once

#include "../object.hpp"
#include "pyvm/pyvm.h"
#include <cstring>
#include <string.h>

PYVM_TOP_LEVEL_NAMESPACE_BEGIN

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

PYVM_TOP_LEVEL_NAMESPACE_END

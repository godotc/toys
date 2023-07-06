#pragma once


#include "../object.hpp"
#include "pyvm/pyvm.h"
#include <cstring>
#include <string.h>

PYVM_TOP_LEVEL_NAMESPACE_BEGIN

class PyString : public PyObject
{
  public:
    PyString(const char *cs)
    {
        m_Value = new char[std::strlen(cs)];
        strcpy(m_Value, cs);
    }
    PyString(const char *cs, int len)
    {
        m_Value = new char[len];
        memcpy(m_Value, cs, len);
    }

    const char *str() { return m_Value; }
    int         length() { return m_Length; }


  private:
    char *m_Value;
    int   m_Length;
};

PYVM_TOP_LEVEL_NAMESPACE_END

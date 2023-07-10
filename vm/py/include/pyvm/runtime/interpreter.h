#pragma once
#include "pyvm/core/code_object.hpp"
#include "pyvm/data_struct/array_list.hpp"
#include "pyvm/object.hpp"
#include "pyvm/pyvm.h"

PYVM_TOP_LEVEL_NAMESPACE_BEGIN


class Interpreter
{
  public:
    void run(CodeObject *codes);


  private:
    PyArrayList<PyObject *> *m_Stack;
    PyArrayList<PyObject *> *m_Consts;
};


PYVM_TOP_LEVEL_NAMESPACE_END

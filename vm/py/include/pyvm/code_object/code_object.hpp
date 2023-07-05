#pragma once


#include "pyvm/data_struct/array_list.hpp"
#include "pyvm/data_struct/string.hpp"
#include "pyvm/object.hpp"
class CodeObject
{
  public:
    CodeObject(int                      argcount,
               int                      nlocals,
               int                      stacksize,
               int                      flag,
               PyString               **bytecodes,
               PyArrayList<PyObject *> *consts,
               PyArrayList<PyObject>   *names,
               PyArrayList<PyObject *> *varNames,
               PyArrayList<PyObject>   *freevars,
               PyArrayList<PyObject *> *cellvars,
               PyString                *co_name,
               int                      lineno,
               PyString                *notable)
    {
    }


  private:
    int m_ArgCount;
    int m_NumLocals;
    int m_StackSize;
    int m_Flag;
};

#pragma once


#include "pyvm/data_struct/array_list.hpp"
#include "pyvm/data_struct/string.hpp"
#include "pyvm/object.hpp"
#include "pyvm/pyvm.h"

PYVM_TOP_LEVEL_NAMESPACE_BEGIN


class CodeObject : public PyObject

{
  public:
    CodeObject(
        int                      argccount,
        int                      nlocals,
        int                      stack_size,
        int                      flag,
        PyString                *bytecodes,
        PyArrayList<PyObject *> *consts,
        PyArrayList<PyObject *> *names,
        PyArrayList<PyObject *> *var_names,
        PyArrayList<PyObject *> *free_vars,
        PyArrayList<PyObject *> *cell_vars,
        PyString                *file_name,
        PyString                *co_name,
        int                      line_number,
        PyString                *notable)
        : m_ArgCount(argccount), m_NumLocals(nlocals), m_StackSize(stack_size), m_Flag(flag),
          m_Bytecodes(bytecodes),
          m_Consts(consts), m_Names(names), m_VarNames(var_names), m_FreeVars(free_vars), m_CellVars(cell_vars),
          m_FileName(file_name), m_CoName(co_name), m_LineNumber(line_number), m_Notable(notable)

    {
    }


  private:
    int                      m_ArgCount;
    int                      m_NumLocals;
    int                      m_StackSize;
    int                      m_Flag;
    PyString                *m_Bytecodes;
    PyArrayList<PyObject *> *m_Consts;
    PyArrayList<PyObject *> *m_Names;
    PyArrayList<PyObject *> *m_VarNames;
    PyArrayList<PyObject *> *m_FreeVars;
    PyArrayList<PyObject *> *m_CellVars;
    PyString                *m_FileName;
    PyString                *m_CoName;
    int                      m_LineNumber;
    PyString                *m_Notable;
};

PYVM_TOP_LEVEL_NAMESPACE_END

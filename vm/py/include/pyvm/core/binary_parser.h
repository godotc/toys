
#pragma once


#include "fmt/core.h"
#include "log.h"
#include "pyvm/core/code_object.hpp"
#include "pyvm/data_struct/array_list.hpp"
#include "pyvm/data_struct/string.hpp"
#include "pyvm/io/buffer_input_stream.hpp"
#include "pyvm/object.hpp"
#include "pyvm/pyvm.h"

PYVM_TOP_LEVEL_NAMESPACE_BEGIN

class BinaryFileParser
{
  public:
    BinaryFileParser(BufferInputStream *stream) { m_Filestream = stream; }

  public:
    CodeObject *parse();

  private:
    CodeObject *get_code_object();


  private:

    PyString                *get_byte_code();
    PyArrayList<PyObject *> *get_consts();
    PyArrayList<PyObject *> *get_names();
    PyArrayList<PyObject *> *get_var_names();
    PyArrayList<PyObject *> *get_free_vars();
    PyArrayList<PyObject *> *get_cell_vars();
    PyString                *get_file_name();
    PyString                *get_name();
    PyString                *get_no_table();


  private:
    PyString                *get_string();
    PyArrayList<PyObject *> *get_tuple();

  private:
    PyArrayList<PyString *> *m_StringTable;

  private:
    BufferInputStream *m_Filestream;
};


PYVM_TOP_LEVEL_NAMESPACE_END

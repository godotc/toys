
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
    CodeObject *parse()
    {
        int magic_number = m_Filestream->read_int();
        LOG_DEBUG("magic number is 0x{:x}", magic_number);
        // m_Filestream->read_int();

        int mod_date = m_Filestream->read_int();
        LOG_DEBUG("mod date is 0x{:x}", mod_date);

        char object_type = m_Filestream->read();
        if (object_type == 'c') {
            CodeObject *result = get_code_object();
            LOG_TRACE("parse successfully!\n");
            return result;
        }

        return nullptr;
    }


  private:
    CodeObject *get_code_object()
    {
        int argcount = m_Filestream->read_int();
        LOG_DEBUG("argcount: {}", argcount);

        int n_locals = m_Filestream->read_int();
        LOG_DEBUG("n_locals: {}", n_locals);

        int stack_size = m_Filestream->read_int();

        int flags = m_Filestream->read_int();
        LOG_DEBUG("flags: {}", flags);

        PyString *byte_code = get_byte_code();

        PyArrayList<PyObject *> *consts    = get_consts();
        PyArrayList<PyObject *> *names     = get_names();
        PyArrayList<PyObject *> *var_names = get_var_names();
        PyArrayList<PyObject *> *free_vars = get_free_vars();
        PyArrayList<PyObject *> *cell_vars = get_cell_vars();

        PyString *file_name   = get_file_name();
        PyString *module_name = get_name();

        int begin_line_number = m_Filestream->read_int();

        PyString *no_table = get_no_table();


        return new CodeObject(argcount, n_locals, stack_size, flags,
                              byte_code,
                              consts, names, var_names, free_vars, cell_vars,
                              file_name, module_name, begin_line_number, no_table);
    }

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
    auto get_string() -> PyString *;
    auto get_tuple() -> PyArrayList<PyObject *> *;


  private:
    BufferInputStream *m_Filestream;
};


PYVM_TOP_LEVEL_NAMESPACE_END

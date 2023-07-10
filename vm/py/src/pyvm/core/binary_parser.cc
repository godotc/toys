#include <pyvm/core/binary_parser.h>

#include "log.h"
#include "pyvm/data_struct/array_list.hpp"
#include "pyvm/data_struct/integer.hpp"
#include "pyvm/data_struct/string.hpp"
#include "pyvm/object.hpp"
#include <assert.h>
#include <cstring>
#include <memory>


PYVM_TOP_LEVEL_NAMESPACE_BEGIN


CodeObject *BinaryFileParser::parse()
{
    int magic_number = m_Filestream->read_int();
    LOG_DEBUG("magic number is 0x{:x}", magic_number);
    // m_Filestream->read_int();

    int mod_date = m_Filestream->read_int();
    LOG_DEBUG("mod date is 0x{:x}", mod_date);

    char object_type = m_Filestream->read();
    LOG_DEBUG("object type is {}", object_type);
    if (object_type == 'c')
    {
        CodeObject *result = get_code_object();
        LOG_TRACE("parse successfully!\n");
        return result;
    }

    LOG_WARN("Not found the `c` sig, not a code_object, exit...");

    return nullptr;
}

CodeObject *BinaryFileParser::get_code_object()
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

auto BinaryFileParser::get_string() -> PyString *
{
    int  len       = m_Filestream->read_int();
    auto str_value = std::make_unique<char>(len);

    for (int i = 0; i < len; ++i) {
        str_value.get()[i] = m_Filestream->read();
    }

    PyString *s = new PyString(str_value.release(), len);

    return s;
}

auto BinaryFileParser::get_tuple() -> PyArrayList<PyObject *> *
{
    int       len = m_Filestream->read_int();
    PyString *str;

    auto list = new PyArrayList<PyObject *>(len);

    for (int i = 0; i < len; ++i) {
        char obj_type = m_Filestream->read();

        switch (obj_type) {
        case 'c':
            LOG_DEBUG("got a code object");
            list->push_back(get_code_object());
            break;
        case 'i':
            list->push_back(new PyInteger(m_Filestream->read_int()));
            break;
        case 'N':
            list->push_back(nullptr);
            break;
        case 't':
            str = get_string();
            list->push_back(str);
            m_StringTable->push_back(str);
            break;
        case 's':
            list->push_back(get_string());
            break;
        case 'R':
            list->push_back(m_StringTable->get(m_Filestream->read_int()));
            break;
        }
    }


    return list;
}

PyString *BinaryFileParser::get_byte_code()
{
    assert(m_Filestream->read() == 's');

    return this->get_string();
}


auto BinaryFileParser::get_consts() -> PyArrayList<PyObject *> *
{
    if (m_Filestream->read() == '(') {
        return get_tuple();
    }

    m_Filestream->unread();

    return nullptr;
}
PyArrayList<PyObject *> *BinaryFileParser::get_names()
{
    if (m_Filestream->read() == 's') {
        return get_tuple();
    }

    m_Filestream->unread();
    return nullptr;
}
PyArrayList<PyObject *> *BinaryFileParser::get_var_names()
{
    if (m_Filestream->read() == 's') {
        return get_tuple();
    }

    m_Filestream->unread();
    return nullptr;
}
PyArrayList<PyObject *> *BinaryFileParser::get_free_vars()
{
    if (m_Filestream->read() == 's') {
        return get_tuple();
    }

    m_Filestream->unread();
    return nullptr;
}
PyArrayList<PyObject *> *BinaryFileParser::get_cell_vars()
{
    if (m_Filestream->read() == 's') {
        return get_tuple();
    }

    m_Filestream->unread();
    return nullptr;
}
PyString *BinaryFileParser::get_file_name()
{
    if (m_Filestream->read() == 's') {
        return get_string();
    }

    m_Filestream->unread();
    return nullptr;
}
PyString *BinaryFileParser::get_name()
{
    if (m_Filestream->read() == 's') {
        return get_string();
    }

    m_Filestream->unread();
    return nullptr;
}

PyString *BinaryFileParser::get_no_table()
{
    if (m_Filestream->read() == 's') {
        return get_string();
    }

    m_Filestream->unread();
    return nullptr;
}

PYVM_TOP_LEVEL_NAMESPACE_END

#include "pyvm/data_struct/array_list.hpp"
#include "pyvm/data_struct/string.hpp"
#include "pyvm/object.hpp"
#include <assert.h>
#include <cstring>
#include <memory>
#include <pyvm/core/binary_parser.h>


PYVM_TOP_LEVEL_NAMESPACE_BEGIN

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
            list->pop_back();
            break;
        case 'c':
            list->pop_back();
            break;
        case 'c':
            list->pop_back();
            break;
        case 'c':
            list->pop_back();
            break;
        case 'c':
            list->pop_back();
            break;
        case 'c':
            list->pop_back();
            break;
        case 'c':
            list->pop_back();
            break;
        case 'c':
            list->pop_back();
            break;
        }
    }


    return list;
}

PYVM_TOP_LEVEL_NAMESPACE_END

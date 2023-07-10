

#include "pyvm/runtime/interpreter.h"
#include "pyvm/core/bytecode.h"
#include "pyvm/data_struct/array_list.hpp"
#include "pyvm/object.hpp"
#include "pyvm/pyvm.h"
#include <pyvm/core/code_object.hpp>

PYVM_TOP_LEVEL_NAMESPACE_BEGIN

void Interpreter::run(CodeObject *codes)
{
    int pc          = 0;
    int code_length = codes->m_Bytecodes->length();


    m_Stack  = new PyArrayList<PyObject *>(codes->m_StackSize);
    m_Consts = codes->m_Consts;

    while (pc < code_length) {
        unsigned char op_code = codes->m_Bytecodes->str()[pc++];

        bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

        int op_arg = -1;
        if (has_argument) {
            int byte1 = (codes->m_Bytecodes->str()[pc++] & 0xFF);
        }
    }
}


PYVM_TOP_LEVEL_NAMESPACE_END

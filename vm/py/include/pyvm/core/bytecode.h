#pragma once
#include "pyvm/pyvm.h"

PYVM_TOP_LEVEL_NAMESPACE_BEGIN

class ByteCode
{
  public:
    using CMD = const unsigned char;

    static CMD PRIMARY_ADD = 23;

    static CMD PRINT_ITEM    = 71;
    static CMD PRINT_NEWLINE = 72;
    static CMD RETURN_VALUE  = 83;

    static CMD HAVE_ARGUMENT = 90;

    // by index  in const list
    static CMD LOAD_CONST = 100;
};

PYVM_TOP_LEVEL_NAMESPACE_END

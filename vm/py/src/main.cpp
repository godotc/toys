#include "log.h"
#include "pyvm/core/binary_parser.h"
#include "pyvm/core/code_object.hpp"
#include <iostream>
#include <pyvm/io/buffer_input_stream.hpp>

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 1) {
        LOG_ERROR("VM need a parameter: filename/filepath");
        return 1;
    }

    pyvm::BufferInputStream stream(argv[1]);
    printf("magic numblr is 0x%x\n", stream.read_int());

    pyvm::BinaryFileParser parser(&stream);
    pyvm::CodeObject      *main_code = parser.parse();

    return 0;
}

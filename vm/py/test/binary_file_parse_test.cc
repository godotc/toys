
#include "pyvm/core/binary_parser.h"
#include "pyvm/io/buffer_input_stream.hpp"
#include <memory>

int main(int charc, char **argv)
{
    auto stream = new pyvm::BufferInputStream(argv[1]);

    pyvm::BinaryFileParser parser(stream);

    parser.parse();

    return 0;
}

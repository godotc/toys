#include "log.h"
#include <iostream>
#include <pyvm/io/buffer_input_stream.hpp>

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 1) {
        LOG_ERROR("VM need a parameter: filename/filepath");
        return 1;
    }

    BufferInputStream stream(argv[1]);
    printf("magic numblr is 0x%x\n", stream.ReadInt());

    return 0;
}

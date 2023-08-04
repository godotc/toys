#include "../../src/utils.h"

#include <stdio.h>


auto main() -> int
{
    auto ret = Command::New("ls")
                   .args("-a")
                   .exec();

    printf("%d\n", ret);
}
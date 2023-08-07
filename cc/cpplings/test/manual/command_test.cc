#include "../../include/command.h"

#include <stdio.h>


auto main() -> int
{
    auto ret = Command::New("ls")
                   .args("-a")
                   .exec();

    printf("%s\n %d\n", ret.output.c_str(), ret.status.success());
}

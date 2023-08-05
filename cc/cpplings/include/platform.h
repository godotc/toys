#pragma once

#include <unistd.h>


#ifdef _WIN32
#else
#include <fcntl.h>
using FD = int;
#endif

namespace platfrom {

void _dup2(const char *f1, const char *f2) {}
void _dup2(FD f1, FD f2)
{
#ifdef _WIN32
#else
    auto fd = open("abc", 0);
    ::dup2(f1, fd);
#endif
}

}; // namespace platfrom

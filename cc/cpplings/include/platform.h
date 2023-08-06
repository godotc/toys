#pragma once


using FD = int;

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
using FD = int;
#endif

namespace platfrom {


void _dup2(FD f1, FD f2)
{
#ifdef _WIN32

#else
    auto fd = open("abc", 0);
    ::dup2(f1, fd);
#endif
}

void popen()
{


    _eopen();
#ifdef _WIN32


#else
#endif
}

}; // namespace platfrom

#pragma once


#include <bits/types/FILE.h>
#include <cstdio>
#include <string>
#include <tuple>
using FD = int;

#ifdef _WIN32
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <unistd.h>
using FD = int;
#endif

namespace platfrom {


using status_code = int;
using output      = std::string;
auto GetCommandOutput(std::string &cmd) -> std::tuple<status_code, output>
{
    FILE *out_pipe =
#ifdef _WIN32
        _popen
#else
        popen
#endif
        ((cmd + " 2>&1").c_str(), "r");


    std::string exec_output;
    int         code;
    if (out_pipe) {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), out_pipe)) {
            exec_output += buffer;
        }
        code =
#ifdef _WIN32
            _popen
#else
            pclose
#endif
            (out_pipe);
    }
    else {
        code = -1;
    }

    return {code, exec_output};
}



void _dup2(FD f1, FD f2)
{
#ifdef _WIN32

#else
    auto fd = open("abc", 0);
    ::dup2(f1, fd);
#endif
}
}; // namespace platfrom

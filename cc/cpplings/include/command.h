#pragma once

// #define todo()
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <vector>


#include "platform.h"



struct Command
{

    struct Status
    {
        friend Command;
        auto success() -> bool { return result == 0; }

      private:
        int result = -1;
    };

    static auto New(const char *in_cmd) -> Command
    {
        Command cmd(in_cmd);
        return cmd;
    }

    Command() = default;
    Command(const char *in_cmd) : _cmd(in_cmd) { space(); }


    auto args(std::string in_args) -> Command &
    {
        _cmd += in_args;
        space();
        return *this;
    }

    auto args(std::vector<std::string> in_args) -> Command &
    {
        for (const auto &arg : in_args) {
            this->args(arg);
        }
        return *this;
    }

    auto exec() -> Command &
    {



        // TODO: redirect the file into my outputs
        FILE *err_pipe = _popen(_cmd.c_str(), "rt");
        FILE *out_pipe = _popen((_cmd + " 2>&1").c_str(), "r");

        if (out_pipe) {
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), out_pipe)) {
                stdout_.get() buffer;
            }
            status.result = _pclose(out_pipe);
        }
        else {
            status.result = -1;
        }
        return *this;
    }

  private:
    auto space() -> void { _cmd.push_back(' '); }

  public:
    Status status;

    // HELPME: when execute's relocate the command out to these 2 file/buffer, then I need to get them somewherer
    typename int stdout_;
    type         stderr_;

  private:
    std::string _cmd;
};

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

    Command() = delete;
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
        auto &&[code, output_] = platfrom::GetCommandOutput(_cmd);
        output                 = output_;
        status.result          = code;

        return *this;
    }

  private:
    auto space() -> void { _cmd.push_back(' '); }

  public:
    Status      status;
    std::string output;

  private:
    std::string _cmd;
};

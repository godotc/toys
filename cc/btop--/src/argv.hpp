#pragma once

#include <cstdio>
#include <format>
#include <iostream>
#include <memory>
#include <string>


template <class T>
struct executeale {
    virtual void execute();
};

using executale_fn = void();

struct Argument : std::enable_shared_from_this<Argument>, executeale<Argument> {

    Argument(auto name, auto flag, auto help, executale_fn func) : name(name), short_flag(flag), help(help) {}

    void display()
    {
        std::cout << std::format("{}\t-{}\tP{}", name, short_flag, help) << std::endl;
    }

    std::string name;
    std::string short_flag;
    std::string help;
};


struct Argument_Help : Argument {
    void execute() override
    {
        std::cout << "hello world" << std::endl;
    }
};

#include "argv.hpp"

#include <algorithm>
#include <ios>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>
#include <numeric>
#include <vector>



class Program
{
  public:

    static Program New(auto name, auto version)
    {
        auto program    = Program();
        program.name    = name;
        program.version = version;
        return std::move(program);
    }

    Program &Arg(auto name, auto flag, auto help, executale_fn func = nullptr)
    {
        return *this;
    }

    bool run(int argc, char **argv)
    {
        return true;
    }



  private:
    std::vector<std::shared_ptr<Argument>> args;
    std::string                            name;
    std::string                            version;
};

int main(int argc, char **argv)
{
    auto program =
        Program::New("btop--", "0.0.1")
            .Arg("help", "h", "this is a help message", []() { std::cout << "hello world" << std::endl; })
            .Arg("help", "h", "this is a help message");


    program.run(argc, argv);

    return 0;
}

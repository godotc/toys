#include <format>
#include <log.h>

int main()
{

    // auto n = "lll";
    // LOG("hello world {}", n);


    // char const *a = "value";
    ERROR("buff {} {}", a);

    auto fmt = "abc {} {} {}";

    std::cout << std::vformat(fmt, std::make_format_args(1, 2, 3)) << std::endl;
}

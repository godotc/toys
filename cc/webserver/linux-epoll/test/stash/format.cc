#include <format>
#include <iostream>
#include <source_location>

using namespace std;

template <class... T>
auto a(format_string<T...> &&fmt, T &&...args)
{
    cout << format(fmt, args...) << '\n';
}

int main()
{
    auto ip   = "0.0";
    auto port = 1234;
    a("Exception occur on accept a new client, ip: {} , port :{}", ip, port);

    // auto fmt = ("hello {}");
    // std::vformat(fmt.get(), std::make_format_args(__args...));
    // a(format_string < char, type_identity_t<decltype(t...)>(fmt), "world");
}

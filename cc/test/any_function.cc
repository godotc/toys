#include <any>
#include <bits/stdc++.h>
#include <cstdio>
#include <functional>
#include <map>
#include <variant>
#include <vector>

using namespace std;


auto print()
{
}

int main()
{
    using FF = function<any(vector<any>)>;
    unordered_map<string, function<any(vector<any>)>> funcs;

    FF f1 = [](auto a) -> any { return a.at(0); };


    funcs["f1"] = f1;



    auto a     = "hello world";
    using type = decltype(a);

    auto ret = funcs["f1"]({a});

    cout << ret.type().name() << '\n';

    cout << any_cast<type>(ret);
}

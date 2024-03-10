#include <bits/stdc++.h>
#include <cstdint>
#include <cstdio>
#include <istream>
#include <memory>

using namespace std;

auto f()
{
    int a = 1;
    return a;
}

auto f1()
{
    char a[] = "hello world abcdefghijklmn";

    // extend lifetime
    return nullptr;
}

auto f2()
{
    const char *a = "hello world abcdefghijklmn";


    printf("%p\n", a);

    cout << &a << endl;
    cout << addressof(a) << endl;
    return a;
}

auto address()
{
    int  v = 0;
    int *a = &v;
    cout << "cout v =" << v << endl;
    cout << "%p";
    printf("=%p\n", a);
    cout << "hex<< (void*)a = " << ostream::hex << ostream::showbase << (void *)a << endl;
    cout << "hex<< (uintptr_t*)a = " << ostream::hex << ostream::showbase << (uintptr_t)a << endl;

    cout << "addressof(a) = " << addressof(a) << endl;
    cout << "&a = " << &a << endl;

    auto b = &a;
    auto c = &b;
    auto d = &c;
    auto e = &d;
    auto f = &e;
    auto g = &f;
    auto h = &g;

    printf("----\n");
    printf("%p\n", b);
    printf("%p\n", c);
    printf("%p\n", d);
    printf("%p\n", e);
    printf("%p\n", f);
    printf("%p\n", g);
    printf("%p\n", h);

    printf("----\n");
    printf("%p\n", &b);
    printf("%p\n", &c);
    printf("%p\n", &d);
    printf("%p\n", &e);
    printf("%p\n", &f);
    printf("%p\n", &g);
    printf("%p\n", &h);

    printf("----\n");
    printf("%p\n", addressof(b));
    printf("%p\n", addressof(c));
    printf("%p\n", addressof(d));
    printf("%p\n", addressof(e));
    printf("%p\n", addressof(f));
    printf("%p\n", addressof(g));
    printf("%p\n", addressof(h));

    printf("----\n");
    std::cout << b << "\n";
    std::cout << c << "\n";
    std::cout << d << "\n";
    std::cout << e << "\n";
    std::cout << f << "\n";
    std::cout << g << "\n";
    std::cout << h << "\n";
}

int main()
{
    auto v = f();

    cout << v << '\n';

    // auto v1 = f1();
    // cout << sizeof(v1) << endl;
    // cout << v1 << '\n'; // not print
    //
    auto v2 = f2();
    cout << sizeof(v2) << endl;
    printf("%p\n", v2);
    cout << &v2 << endl;
    cout << addressof(v2) << endl;
    cout << v2 << '\n'; // not print
                        //
    cout << "\n";

    address();

    return 0;
}

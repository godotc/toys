#include <bits/stdc++.h>
#include <cstdio>
#include <unordered_map>


using namespace std;


class MyClass
{
  public:
    MyClass() = default;
    MyClass(int a) : m_a(a) { cout << "Construct....\n"; }
    ~MyClass() { cout << "Destruct....\n"; }

    int m_a;
};

int main()
{
    {
        unordered_map<string, MyClass> mp;

        MyClass a('a');


        mp["a"] = a;


        MyClass b('b');

        mp["a"] = b;
    }

    getchar();

    return 0;
}

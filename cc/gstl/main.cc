#include "vector.hpp"


int main()
{
    gstl::vector<int> v;

    v.push_back(1);

    auto a = v.pop_back();

    std::cout << a << std::endl;


    v.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});

    for (int i = 0; i < 11; ++i) {
        std::cout << v.pop_back() << std::endl;
    }

    return 0;
}

#include <functional>

#include <iostream>



int main()
{

    std::function<float()> f1 = []() -> int { return 2; };
    std::function<void()>  f2;

    // f1 = []() -> int { return 1; };
    f2 = []() -> float { return 2.5; };

    std::cout << f1.target<int()>() << " " << f2.target<float()>() << std::endl;
    std::cout << f1() << std::endl;
}

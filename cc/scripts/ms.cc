#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    auto size = 10;
    if (argc > 1) {
        size = atoi(argv[1]);
    }

    std::random_device rd;
    std::mt19937       gen32{rd()};

    std::uniform_int_distribution<int> x(0, 100);


    for (int i = 0; i < size; ++i)
    {
        cout << x(gen32) << endl;
    }


    return 0;
}

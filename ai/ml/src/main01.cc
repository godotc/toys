#include "print.h"
#include <bits/stdc++.h>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>

// clang-foramt off
std::vector<std::array<float, 2>> train = {
    {0, 0},
    {1, 2},
    {2, 4},
    {3, 6},
    {4, 8}
};
// clang-foramt on



auto train_count()
{
    return train.size() * train[0].size();
}

auto random_float()
{
    return rand() / (float)RAND_MAX;
}

// y =x1*w1 + x2*2 ... +b
auto cost(float w, float b, bool debug = false)
{
    float result = 0.f;
    for (auto &values : train) {
        auto [x, p] = values;
        float y     = x * w;
        float d     = y - p;
        result += d * d;

        if (debug)
            print("expected %f |  actual %f", p, y);
    }
    result /= train_count();
    return result;
}

int main()
{
    srand(42);

    float w = random_float() * 10.f;
    float b = random_float() * 4.2f;
    print(w);

    print(train_count());

    float mw, mb;

    float rate = 1e-3;
    float eps  = 1e-3;

    float dcost = (cost(w + eps, b) - cost(w, b)) / eps;
    print(dcost);

    auto origin = cost(w, b);

    for (int i = 0; i < 180; ++i) {

        float dw = cost(w + eps, b) - cost(w, b) / eps;
        float db = cost(w, b + eps) - cost(w, b) / eps;

        w -= rate * dw;
        b -= rate * db;

        print("cost w = %f, w = %f", dw, w);
        print("cost b = %f, w = %f", db, b);
    }
    print("--------------------------------");
    print(origin);
    print(cost(w, 1));


    print("--------------------------------");

    return 0;
}

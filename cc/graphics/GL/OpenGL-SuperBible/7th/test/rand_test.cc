#include <gtest/gtest.h>
#include <log.h>
#include <random>
#include <vector>



TEST(TestRandFunc, should_be_differenet_on_every_call)
{

    // wrong segment
    // auto get_random_float = [](auto min, auto max) -> float {
    // srand(time(nullptr));
    //     auto d = abs(max - min);
    //     return rand() / (float)RAND_MAX * d - max;
    // };

    srand(time(nullptr));

    auto get_random_float = [](auto min, auto max) -> float {
        auto d = abs(max - min);
        return rand() / (float)RAND_MAX * d - max;
    };

    std::set<float> ns;
    for (int i = 0; i < 10; ++i) {
        auto n = get_random_float(-1, 1);
        ASSERT_EQ(ns.find(n), ns.end());
        ns.insert(n);
    }

    for (auto f : ns) {
        LOG_DEBUG("{}", f);
    }
}

#include <algorithm>
#include <bits/ranges_algo.h>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>

using namespace std;

void test_pq()
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;

    int a[] = {1, 2, 3};
    std::make_heap(std::begin(a), std::end(a), [](auto a, auto b) { return a < b; });

    pq.push(1);
    pq.push(5);
    pq.push(2);

    for (int i = 0; i < 3; ++i) {
        auto x = pq.top();
        std::cout << x << '\n';
        pq.pop();
    }
}

void test_std_make_heap()
{
    int a[] = {5, 4, 7, 3, 1};

    auto f = [](int a, int b) { return a > b; };
    std::make_heap(std::begin(a), end(a), f);

    for (auto x : a) {
        cout << x << ' ';
    }
    cout << endl;

    std::sort_heap(std::begin(a), end(a), f);
    for (auto x : a) {
        cout << x << ' ';
    }
    cout << endl;
}

int main()
{

    test_std_make_heap();

    return 0;
}

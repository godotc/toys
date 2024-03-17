#include "any_out.h"
#include <vector>

int main()
{
    std::vector<int> v(5, 10);
    out(v.size());
    for (auto x : v) {
        out(x);
    }

    out("-----------------------------------------------");
    v.resize(15);
    out(v.size());

    for (auto x : v) {
        out(x);
    }

    return 0;
}

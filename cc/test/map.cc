
#include <initializer_list>
#include <map>
#include <utility>


template <class K, class V>
struct Mpair
{
    K k;
    V v;
};

template <class K, class V>
std::map<K, V> create(Mpair<K, V> pairs...)
{
    std::map<K, V> mp;

    for (auto [k, v] : pairs) {
        mp.insert(k, v);
    }

    return mp;
}

template <class K, class V>
std::map<K, V> create(std::initializer_list<std::pair<K, V>> &&list)
{
    std::map<K, V> mp;

    for (auto &&[k, v] : list) {
        mp.insert(k, v);
    }

    return mp;
}

int main()
{
    auto mp = create<int, int>({{1, 2}, {3, 4}});
}

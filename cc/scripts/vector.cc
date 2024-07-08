

#include <cstdio>
#include <vector>

int main() {

  std::vector<int> a;
  for (int i = 0; i < 10; ++i) {
    a.emplace_back(i);
  }
  printf("%d\n", a.size());
  a.clear();
  printf("%d\n", a.size());

  for (int i = 0; i < 10; ++i) {
    printf("%d,", a[i]);
  }
}

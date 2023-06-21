#include <bits/stdc++.h>
#include <condition_variable>
#include <cstdio>
#include <memory>
#include <semaphore.h>
#include <semaphore>
#include <thread>
#include <tuple>

using namespace std;



class Task
{
  public:
    auto operator()(bool &isRunning)
    {
        printf("hello world\n");

        // auto t = make_unique<Task>();

        while (isRunning) {
            // t.get();
        }

        printf("end...\n");
    }

  private:
};

int main()
{
    bool isRunning = true;

    std::thread t(Task(), ref(isRunning));
    t.detach();



    while (1)
    {
        auto c = getchar();
        if (c == 'q') {
            isRunning = !isRunning;
            break;
        }
    }

    return 0;
}

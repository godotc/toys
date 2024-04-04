


#include "sleep.hpp"
#include <cerrno>
#include <chrono>
#include <coroutine>
#include <cstdio>
#include <exception>

#include "condition.hpp"
#include "schelder.hpp"
#include "ut.hpp"


#include <sys/epoll.h>
#include <unistd.h>

using namespace std::chrono_literals;



Task<void> nobody()
{
    debug(), "nobody";
    co_return;
}
Task<int> world()
{
    debug(), "world";
    // throw std::runtime_error("world failed");
    // throw "world failed"; this cannot print what() by exception handler
    co_return 42;
}

Task<int> hello()
{
    auto i = co_await world();
    co_await nobody();

    co_await co_async::sleep_for(1s);
    debug(), "hello";
    co_return i + 1;
}
Task<int> hello2()
{
    auto i = co_await world();

    co_await co_async::sleep_for(2ms);
    debug(), "hello2";
    co_return i + 1;
}


Task<int> print_1()
{
    debug(), 1;

    debug(), __FUNCITON_NAME__;
    co_return 1;
}
Task<int> print_2()
{
    debug(), 2;
    debug(), __FUNCITON_NAME__;
    co_return 2;
}

Task<int> test_wait_all()
{
    debug(), "begin of ", __FUNCITON_NAME__;
    auto [i, j, k] = co_await when_all(print_1(), print_2(), print_2());

    co_return j + k;
}
Task<int> test_wait_any()
{
    debug(), "begin of ", __FUNCITON_NAME__;
    auto var = co_await when_any(print_1(), print_2(), print_2());
    debug(), "var index: ", var.index() + 1, "  OK";
    co_return std::get<0>(var);
}

void test_before_epolls()
{
    try {

        // normal tasks
        //{
        //    Task t  = hello();
        //    auto t2 = hello2();
        //    auto t3 = hello2();
        //    GetScheduler().AddTask(t);
        //    GetScheduler().AddTask(t2);
        //    GetScheduler().AddTask(t3);

        //    GetScheduler().RunAll();
        //    debug(), t.m_Coroutine.promise().result();
        //    debug(), t2.m_Coroutine.promise().result();
        //}

        // when all
        // auto t = test_wait_all();
        auto t = test_wait_any();
        GetTimerLoop().RunAll(t);
        debug(), t.m_Coroutine.promise().result();
    }
    catch (std::exception &excep) {
        debug(), excep.what();
    }
}


#include "fcntl.h"
#include <errno.h>
#include <string.h>

int main()
{
    // nonblock stdin
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flags);



    int epfd = epoll_create1(0);

    struct epoll_event event;
    event.events  = EPOLLIN;
    event.data.fd = STDIN_FILENO;

    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    struct epoll_event ebuffers[16];
    while (1) {
        int ret = epoll_wait(epfd,
                             ebuffers,
                             sizeof(ebuffers) / sizeof(ebuffers[0]),
                             1000); // -1 wait forever
        if (-1 == ret) {
            // debug(), "epoll wait error";
            perror("epoll wait error");
        }
        if (0 == ret) {
            debug(), "epoll timeout";
        }

        for (int i = 0; i < ret; ++i)
        {
            auto fd = ebuffers[i].data.fd;
            while (1) {
                char c;
                int  len = read(fd, &c, 1);
                if (len <= 0) {
                    if (errno == EAGAIN) {
                        // debug(), "read: nothing";
                        break;
                    }
                    if (errno == EWOULDBLOCK) {
                        debug(), "read: would block?";
                        break;
                    }
                    debug(), "read: ", errno, " ", strerror(errno);
                }
                debug(), c;
            }
        }
    }


    return 0;
}

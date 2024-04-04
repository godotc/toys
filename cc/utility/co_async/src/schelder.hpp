#pragma once

#include <cerrno>
#include <chrono>
#include <coroutine>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <sys/epoll.h>
#include <thread>
#include <unistd.h>

#include "base.hpp"
#include "rbtree.hpp"
#include "ut.hpp"



struct SleepUntilPromise
    : public RBTree<SleepUntilPromise>::RBNode,
      public Promise<void> {

    std::chrono::system_clock::time_point m_ExpireTime;

    Promise &operator=(Promise &&) = delete;

    auto get_return_object()
    {
        return std::coroutine_handle<SleepUntilPromise>::from_promise(*this);
    }

    friend bool operator<(SleepUntilPromise const &lhs, SleepUntilPromise const &rhs) noexcept
    {
        return lhs.m_ExpireTime < rhs.m_ExpireTime;
    }
};


struct TimerLoop {
    // did not owning the promise?
    RBTree<SleepUntilPromise> m_TimeerRBTree{};

    TimerLoop &operator=(TimerLoop &&) = delete;

    void AddTimer(SleepUntilPromise &promise)
    {
        m_TimeerRBTree.insert(promise);
    }

    void RunAll(std::coroutine_handle<> coroutine)
    {
        while (!coroutine.done()) {
            coroutine.resume();
            while (!m_TimeerRBTree.empty()) {
                if (!m_TimeerRBTree.empty()) {
                    auto  now     = std::chrono::system_clock::now();
                    auto &promise = m_TimeerRBTree.front();
                    if (promise.m_ExpireTime < now) {
                        m_TimeerRBTree.erase(promise);
                        std::coroutine_handle<SleepUntilPromise>::from_promise(promise).resume();
                    }
                    else {
                        std::this_thread::sleep_until(promise.m_ExpireTime);
                    }
                }
            }
        }
    }
};


// TODO: don't know what will happen
inline TimerLoop &GetTimerLoop()
{
    static TimerLoop scheduler;
    return scheduler;
}



struct EpollFilePromise : Promise<void> {
    int      m_FileNum;
    uint32_t m_Events;

    auto get_return_object()
    {
        return std::coroutine_handle<EpollFilePromise>::from_promise(*this);
    }

    EpollFilePromise &operator=(EpollFilePromise &&) = delete;
};


struct EpollLoop {
    int m_epfd;

    EpollLoop &operator=(EpollLoop &&) = delete;
    EpollLoop()
    {
        m_epfd = epoll_create1(0);
        if (-1 == m_epfd) {
            perror("create epoll fd");
        }
    }
    ~EpollLoop() { close(m_epfd); }


    void AddListener(EpollFilePromise &promise)
    {
        struct epoll_event event;
        event.events   = promise.m_Events;
        event.data.ptr = &promise;
        int ret        = epoll_ctl(m_epfd, EPOLL_CTL_ADD, promise.m_FileNum, &event);
        if (-1 == ret) {
            perror("add new listenser in EpollLoop");
        }
    }

    void TryRun()
    {
        struct epoll_event ebuffers[10];

        while (1) {

            int ret = epoll_wait(
                m_epfd,
                ebuffers,
                sizeof(ebuffers) / sizeof(ebuffers[0]),
                5000); // 0.5
            if (-1 == ret) {
                perror("epoll wait in epoll loop");
            }

            for (int i = 0; i < ret; ++i) {
                // auto task = std::coroutine_handle<EpollFilePromise>::from_address(
                //     ebuffers[i].data.ptr);

                // int  fd = task.promise().m_FileNum;
                // char buf[512];
                // while (1) {
                //     int ret = read(fd, buf, sizeof(buf));
                //     if (ret <= 0) {
                //         if (ret == EAGAIN) {
                //             break;
                //         }
                //         debug(), "error on reading the fd: ", fd,
                //             " error: ", strerror(errno);
                //     }
                // }
                auto &promise = *static_cast<EpollFilePromise *>(
                    ebuffers[i].data.ptr);
                if (-1 == epoll_ctl(m_epfd, EPOLL_CTL_DEL,
                                    promise.m_FileNum, nullptr)) {
                    perror("delete a listening fileno");
                }
                std::coroutine_handle<EpollFilePromise>::from_promise(promise).resume();
            }
        }
    }
};

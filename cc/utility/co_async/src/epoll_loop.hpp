#pragma once


#include "base.hpp"
#include "ut.hpp"
#include <cerrno>
#include <chrono>
#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <fcntl.h>
#include <iterator>
#include <optional>
#include <span>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <system_error>
#include <unistd.h>


namespace co_async {

using EpollEventMask = std::uint32_t;


inline int check_error(int ret)
{
    if (-1 == ret) [[unlikely]]
    {
        throw std::system_error(errno, std::system_category());
    }
    return ret;
}

auto check_error_nonblock(auto ret, int blockers = 0, int blockerr = EWOULDBLOCK)
{
    if (ret == -1) {
        if (errno != blockerr) [[unlikely]] {
            throw std::system_error(errno, std::system_category());
        }
        ret = blockers;
    }
    return ret;
}

struct EpollLoop;
struct EpollFileAwaiter;

struct EpollFilePromise : public Promise<EpollEventMask> {

    struct EpollFileAwaiter *m_Awaiter = nullptr;


    EpollFilePromise &operator=(EpollFilePromise &&) = delete;
    inline ~EpollFilePromise();

    auto get_return_object()
    {
        return std::coroutine_handle<EpollFilePromise>::from_promise(*this);
    }
};


struct EpollLoop {
    int                m_epfd;
    std::size_t        m_Count = 0;
    struct epoll_event m_EventBuffers[64];

    EpollLoop()
    {
        m_epfd = epoll_create1(0);
        if (-1 == m_epfd) {
            perror("create epoll fd");
        }
    }
    ~EpollLoop() { close(m_epfd); }

    EpollLoop &operator=(EpollLoop &&) = delete;


    inline bool AddListener(EpollFilePromise &promise, int EP_ControlCode);
    inline void RemoveListener(int fileno);
    inline bool TryRun(std::optional<std::chrono::system_clock::duration> timeout);
};


inline EpollLoop &GetEpollLoop()
{
    static EpollLoop loop;
    return loop;
}


struct EpollFileAwaiter {
    using clock_type = std::chrono::system_clock;

    EpollLoop     &m_Loop;
    int            m_FileNum;
    EpollEventMask m_Events;

    EpollEventMask m_ResumeEvents;
    int            m_ControlCode = EPOLL_CTL_ADD;

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<EpollFilePromise> coroutine)
    {
        auto &promise     = coroutine.promise();
        promise.m_Awaiter = this;
        if (!m_Loop.AddListener(promise, m_ControlCode)) {
            promise.m_Awaiter = nullptr;
            coroutine.resume();
        }
    }
    EpollEventMask await_resume() const noexcept { return m_ResumeEvents; }
};


EpollFilePromise::~EpollFilePromise()
{
    if (m_Awaiter) {
        m_Awaiter->m_Loop.RemoveListener(m_Awaiter->m_FileNum);
    }
}

bool EpollLoop::AddListener(EpollFilePromise &promise, int EP_ControlCode)
{
    struct epoll_event event;
    // assign the listening event types
    event.events   = promise.m_Awaiter->m_Events;
    event.data.ptr = &promise;
    int ret        = epoll_ctl(m_epfd,
                               EP_ControlCode,
                               promise.m_Awaiter->m_FileNum,
                               &event);
    if (-1 == ret) {
        return false;
    }
    if (EP_ControlCode == EPOLL_CTL_ADD) {
        ++m_Count;
    }
    return true;
}

void EpollLoop::RemoveListener(int fileno)

{
    check_error(epoll_ctl(m_epfd, EPOLL_CTL_DEL, fileno, nullptr));
    --m_Count;
}

bool EpollLoop::TryRun(std::optional<std::chrono::system_clock::duration> timeout)
{
    if (m_Count == 0) {
        return false;
    }

    int timeout_in_ms =
        timeout ? std::chrono::duration_cast<std::chrono::milliseconds>(*timeout).count()
                : -1;


    int ret = check_error(epoll_wait(m_epfd, m_EventBuffers, std::size(m_EventBuffers), timeout_in_ms));

    for (int i = 0; i < ret; ++i) {
        auto &event   = m_EventBuffers[i];
        auto &promise = *static_cast<EpollFilePromise *>(event.data.ptr);

        // assign the incoming events type
        promise.m_Awaiter->m_ResumeEvents = event.events;
    }

    for (int i = 0; i < ret; ++i) {
        auto &promise = *static_cast<EpollFilePromise *>(m_EventBuffers[i].data.ptr);
        std::coroutine_handle<EpollFilePromise>::from_promise(promise)
            .resume();
    }
    return true;
}



class [[nodiscard]] AsyncFile
{
    int m_FileNum;

  public:
    explicit AsyncFile(int fileno) noexcept : m_FileNum(fileno) {}
    AsyncFile(AsyncFile &&other) noexcept
        : m_FileNum(other.m_FileNum)
    {
        other.m_FileNum = -1;
    }

    AsyncFile &operator=(AsyncFile &&other) noexcept
    {
        std::swap(m_FileNum, other.m_FileNum);
        return *this;
    }

    int FileNum() const noexcept { return m_FileNum; }

    int ReleaseOwnerShip() noexcept
    {
        int ret   = m_FileNum;
        m_FileNum = -1;
        return ret;
    }

    void SetNonBlock() const
    {
#if 1
        int attr = 1;
        check_error(ioctl(FileNum(), FIONBIO, &attr));
#else
        int flags = fcntl(FileNum(), F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(FileNum(), F_SETFL, flags);
#endif
    }
};


inline Task<EpollEventMask, EpollFilePromise>
wait_file_event(EpollLoop &loop, AsyncFile &file, EpollEventMask events)
{
    co_return co_await EpollFileAwaiter(loop, file.FileNum(), events);
}


inline std::size_t read_file_sync(AsyncFile &file, std::span<char> buffer)
{
    return check_error_nonblock(
        read(file.FileNum(), buffer.data(), buffer.size()));
}

inline std::size_t write_file_sync(AsyncFile &file, std::span<char> buffer)
{
    return check_error_nonblock(
        write(file.FileNum(), buffer.data(), buffer.size()));
}

inline Task<std::size_t>
read_file(EpollLoop &loop, AsyncFile &file, std::span<char> buffer)
{
    co_await wait_file_event(loop, file, EPOLLIN | EPOLLRDHUP);
    std::size_t len = read_file_sync(file, buffer);
    co_return len;
}

inline Task<std::size_t>
write_file(EpollLoop &loop, AsyncFile &file, std::span<char> buffer)
{
    co_await wait_file_event(loop, file, EPOLLOUT | EPOLLHUP);
    std::size_t len = write_file_sync(file, buffer);
    co_return len;
}


} // namespace co_async

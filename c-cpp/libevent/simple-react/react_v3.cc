#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#include "log.h"
pthread_mutex_t LOG_MTX;

void log_lock(bool is_unlocked, void* udata);
void InitLog();

struct Context
{
    int sockfd;
    int epollfd;
};

auto InitAddr(const char* ip, int port)
{
    auto addr        = new sockaddr_in;
    addr->sin_family = AF_INET;
    addr->sin_port   = htons(port);
    ::inet_pton(AF_INET, ip, &addr->sin_addr.s_addr);

    log_debug("initing address...");

    return addr;
}

auto TcpServerStart(sockaddr_in* addr)
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sockfd == -1) perror("socket");

    int opt;
    ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int ret = ::bind(sockfd, (sockaddr*)addr, sizeof(*addr));
    if (ret == -1) perror("bind");

    ::listen(sockfd, 128);

    log_debug("starting socket listen...");

    return sockfd;
}

void handleEvent(epoll_event* event, int listenfd)
{
    char clientip[1024];

    auto context = (Context*)event->data.ptr;
    auto epollfd = context->epollfd;
    auto sockfd  = context->sockfd;

    auto this_ev = event->events;

    if (this_ev & EPOLLIN)
    {
        if (sockfd == listenfd) {

            sockaddr_in client_addr;
            socklen_t   len = sizeof(client_addr);

            int cfd = ::accept4(listenfd, (sockaddr*)&client_addr, &len, SOCK_NONBLOCK);
            if (cfd == -1) perror("accet");

            ::log_debug("New client, ip: %s, port: %d\n",
                        ::inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, clientip, sizeof(clientip)),
                        ::ntohs(client_addr.sin_port));

            epoll_event event_;

            Context* context = new Context;
            context->sockfd  = cfd;
            context->epollfd = epollfd;

            event_.data.ptr = context;
            event_.events   = EPOLLOUT | EPOLLIN;

            int ret = ::epoll_ctl(epollfd, EPOLL_CTL_ADD, cfd, &event_);

            if (ret == -1) perror("Error when add new epoll event(after accept)");
        }
        else {
            int  cfd = sockfd;
            char buf[1024];
            int  n;
            while ((n = ::recv(cfd, buf, sizeof(buf), 0)) && n > 0)
            {
                printf("%s \n", buf);
                ::send(cfd, buf, strlen(buf), 0);
            }
        }
    }


    else if (this_ev & EPOLLOUT)
    {
    }
}

void EventLoop(int listenfd)
{
    int epollfd = ::epoll_create(1);

    // init contex
    Context* contex = new Context;
    contex->sockfd  = listenfd;
    contex->epollfd = epollfd;


    epoll_event event;
    // event.data.fd = sockfd;
    event.data.ptr = &contex;
    event.events   = EPOLLIN | EPOLLOUT;

    ::epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

    epoll_event events[1024];

    while (true) {
        int nums = ::epoll_wait(epollfd, events, 1024, -1);

        for (int i = 0; i < nums; ++i)
        {
            epoll_event* event = events + i;

            handleEvent(event, listenfd);
        }
    }
}



int main(int argc, char* argv[])
{
    InitLog();

    const char* ip = "127.0.0.1";

    auto addr = InitAddr(ip, atoi(argv[1]));

    auto listenfd = TcpServerStart(addr);

    EventLoop(listenfd);

    pthread_mutex_destroy(&LOG_MTX);
    return 0;
}


void log_lock(bool is_unlocked, void* udata)
{
    auto LOCK = static_cast<pthread_mutex_t*>(udata);
    if (is_unlocked)
        pthread_mutex_lock(LOCK);
    else
        pthread_mutex_unlock(LOCK);
}
void InitLog()
{
    log_set_level(0);
    log_set_quiet(0);

    pthread_mutex_init(&LOG_MTX, nullptr);
    log_set_lock(log_lock, &LOG_MTX);
}

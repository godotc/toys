
#include <bits/types/struct_timeval.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_compat.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/listener.h>
#include <event2/util.h>

//
#include <hiredis/adapters/libevent.h>
#include <hiredis/async.h>

//
#include <hiredis/hiredis.h>
#include <hiredis/read.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "utils.h"

redisAsyncContext* redis_conn;

void connectCallback(const redisAsyncContext* c, int status)
{
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("redis Connected...\n");
}
void disconnectCallback(const redisAsyncContext* c, int status)
{
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("redis Disconnected...\n");
}


static void on_read(struct bufferevent* bev, void* arg)
{
    struct evbuffer* evbuf = bufferevent_get_input(bev);

    char* msg = evbuffer_readln(evbuf, nullptr, EVBUFFER_EOL_CRLF);

    if (!msg) return;
    printf("server read the data: %s\n", msg);

    Arg* args = split_req(msg);
    delete msg;


    printf("cmd=%s, element_size=%ld\n", args->str, args->elements);

    Command* c = get_command(args->str);
    if (!c) {
        printf("invalid command!\n");
        free_req(args);
        return;
    }

    printf("---------------------------------------\n");
    printf("%s\n invoking cb...\n\n", c->description);
    c->cb(bev, args);  // args: login account passwd


    delete args;
}
static void on_error(bufferevent* bev, short events, void* arg)
{
    if (events & BEV_EVENT_EOF)  // read == 0
        printf("connection closed\n");
    else if (events & BEV_EVENT_ERROR)  // sterro(erro)
        printf("internal error\n");
    else if (events & BEV_EVENT_TIMEOUT)  //
        printf("timeout\n");

    bufferevent_free(bev);  // close(fd)
}


static void on_open(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg)
{
    char ip[32] = {0};
    evutil_inet_ntop(AF_INET, &((sockaddr_in*)sock)->sin_addr.s_addr, ip, sizeof(ip));
    printf("accept a client fd:%d ip:%s\n", fd, ip);

    struct event_base*  base = (struct event_base*)arg;
    struct bufferevent* bev  = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, on_read, nullptr, on_error, nullptr);
    bufferevent_enable(bev, EV_READ | EV_PERSIST);
}


int main()
{
    struct event_base* base = event_base_new();

    struct sockaddr_in sin;
    std::memset(&sin, '\0', sizeof(sin));
    sin.sin_family      = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port        = htons(8989);
    puts("server listen port: 8989\n");

    // redis
    redisOptions options = {0};
    REDIS_OPTIONS_SET_TCP(&options, "127.0.0.1", 6379);  // ip port
    struct timeval tv       = {0};
    tv.tv_sec               = 1;
    options.connect_timeout = &tv;  // timeout

    redis_conn = redisAsyncConnectWithOptions(&options);
    if (redis_conn->err) {
        printf("Error: %s\n", redis_conn->errstr);
        return 1;
    }

    redisLibeventAttach(redis_conn, base);
    redisAsyncSetConnectCallback(redis_conn, connectCallback);
    redisAsyncSetDisconnectCallback(redis_conn, disconnectCallback);
    /****** redis *****/


    struct evconnlistener* list =
        evconnlistener_new_bind(
            base, on_open, base,
            LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
            512, (sockaddr*)&sin, sizeof(sin));
    // backlog 就绪队列长度
    // flag 属性：端口复用等等


    event_base_dispatch(base);  // loop
    event_base_free(base);
}

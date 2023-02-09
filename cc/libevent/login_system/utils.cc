#include "utils.h"

#include <event2/bufferevent.h>
#include <hiredis/async.h>
#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <string.h>

#include <cstddef>
#include <cstdio>
#include <ctime>

extern redisAsyncContext* redis_conn;

struct replyContext
{
    bufferevent* bev;
    struct Arg*  args;
};

Command commandTable[] =
    {{"login", on_login, "login: please provide name and passwd"},
     {"heart", on_heartbeat, "heart: heart beat"}};


Arg* split_req(char* msg)
{
    char* temp = strtok(msg, " ");
    if (!temp) return nullptr;

    Arg* req = new Arg[sizeof(*req)];
    if (!req) return nullptr;

    req->len = strlen(temp);
    req->str = new char[req->len + 1];

    if (!req->str) return nullptr;
    memset(req->str, '\0', req->len + 1);
    memcpy(req->str, temp, req->len);
    req->elements = 0;

    Arg* tail = req;

    while ((temp = strtok(NULL, " ")))
    {
        Arg* element = new Arg[sizeof(*element)];
        if (!element) return nullptr;

        element->len = strlen(temp);
        element->str = new char[element->len + 1];
        if (!element->str) return nullptr;

        memset(element->str, '\0', element->len + 1);
        memcpy(element->str, temp, element->len);

        tail->next    = element;
        element->next = nullptr;

        tail = element;
        ++req->elements;
    }

    return req;
}

void free_req(Arg* args)
{
    int i;
    if (args->elements > 0) {
        Arg* p = args->next;
        while (p) {
            Arg* t = p;
            free(p->str);
            free(t);

            p = p->next;
        }
    }
    free(args->str);
    free(args);
}

Command* get_command(const char* cmd)
{

    int j;
    int numcommands = sizeof(commandTable) / sizeof(Command);

    for (j = 0; j < numcommands; ++j) {
        Command* c = commandTable + j;
        if (strcmp(cmd, c->name) == 0) {
            return c;
        }
    }

    return nullptr;
}


static void reply_error(bufferevent* bev, const char* errstr)
{
    printf("%s\n", errstr);
    bufferevent_write(bev, errstr, strlen(errstr));
}

// tpedef void (redisCallBackFn)(struct redisAsyncContext*,void*,)
void redis_login_hget(redisAsyncContext* c, void* rpl, void* privdata)
{
    redisReply* reply = (redisReply*)rpl;
    printf("redis_login_hget: %d\n", reply->type);

    // 如果 不存在 直接注册 TODO(fix)

    auto rc = static_cast<replyContext*>(privdata);

    if (reply->elements == 0) {
        const char* name   = rc->args->str;
        const char* passwd = rc->args->next->str;
        redisAsyncCommand(
            redis_conn,
            nullptr, nullptr,
            "hmset role:%s name %s password %s", name, name, passwd);

        const char* suc_msg = "注册成功\n";
        bufferevent_write(rc->bev, suc_msg, strlen(suc_msg));
    }
    else {
        // TODO(fix): check the passwd
        const char* suc_msg = "登陆成功\n";
        bufferevent_write(rc->bev, suc_msg, strlen(suc_msg));
    }

    free_req(rc->args);
    free(rc);
}

void on_login(bufferevent* bev, Arg* args)
{
    printf("on_login\n");

    if (args->elements < 2) {
        reply_error(bev, "on_login 参数不足\n");
        return;
    }

    Arg* params = args->next;

    const char* name       = params->str;
    size_t      name_len   = params->len;
    const char* passwd     = params->next->str;
    size_t      passwd_len = params->next->len;

    printf("name =%s, passwd = %s %ld\n", name, passwd, passwd_len);


    replyContext* rc = new replyContext{bev, args->next};

    redisAsyncCommand(redis_conn, redis_login_hget, rc, "hgetall role:%s", name);

    // 不能在此释放 因为是 async 操作
    // free_req(rc->args);
    // free(rc);
}

void on_heartbeat(bufferevent* bev, Arg* args)
{
    printf("on heartbeat\n");
}

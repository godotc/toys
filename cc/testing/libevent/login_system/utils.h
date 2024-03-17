#ifndef _G_EVD_H_
#define _G_EVD_H_

#include <event2/bufferevent.h>
#include <string.h>

#include <cstddef>

struct Arg
{
    size_t len;
    char*  str;
    size_t elements;
    Arg*   next;
};

struct Command
{
    const char* name;
    void (*cb)(bufferevent*, Arg*);
    const char* description;
};



Arg* split_req(char* msg);

void free_req(Arg*);

Command* get_command(const char* cmd);

void on_login(bufferevent* bev, Arg* args);
void on_heartbeat(bufferevent* bev, Arg* args);


#endif

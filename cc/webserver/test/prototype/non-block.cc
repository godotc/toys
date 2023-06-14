/**
 * @TODO
 * - [x] noblock lfd
 * - [x] accept and read data pack
 * - [ ] read the cfd with non-block
 * - [ ] manage client fd
 * - [ ] multiple thread server
 */

#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <error.h>
#include <fcntl.h>
#include <format>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <tuple>
#include <unistd.h>
#include <unordered_map>

#include <log.h>

#include <thread>
#include <vector>

auto check_perr(int val, int non_expect, auto msg)
{
    if (val == non_expect) {
        perror(msg);
    }
}


static const int                            PORT = 9527;
static std::vector<int>                     client_fds;
static std::unordered_map<int, sockaddr_in> client_infos;

/**
 * @return
 * 1. ip : string
 * 2. port : int
 */
std::tuple<const char *, int> get_info(int fd)
{
    if (client_infos.find(fd) != client_infos.end()) {

        auto client_addr = client_infos[fd];
        char buf[256];

        auto ip   = inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, buf, sizeof(buf));
        auto port = ntohs(client_addr.sin_port);

        DEBUG("two address of inet_ntop, {:x} -> {:x}", ip, buf);



        return std::tuple(ip, port);
    }

    return {};
}


void on_new_client(int lfd, auto &client_fds, auto &client_infos)
{
    sockaddr_in client_addr;
    socklen_t   client_addr_size = sizeof(client_addr);

    // Set the socket to blocking mode
    // int flags = fcntl(lfd, F_GETFL, 0);
    // fcntl(lfd, F_SETFL, flags & ~O_NONBLOCK);

    static char buf[256];
    int         cfd  = accept(lfd, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_size);
    auto        ip   = inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, buf, sizeof(buf));
    auto        port = ntohs(client_addr.sin_port);

    // Restore the socket to non-blocking mode
    // fcntl(lfd, F_SETFL, flags);

    if (cfd == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            // No new client, do nothing
        }
        else if (errno == ENOSPC) {
            ERROR("Maximum number of connections reached. Unable to accept new client.");
        }
        else {
            ERROR("Exception occurred while accepting a new client, ip: {}, port: {}: {}", ip ? ip : "unknown", port, strerror(errno));
        }
    }
    else {
        client_fds.push_back(cfd);
        client_infos.insert({cfd, client_addr});
        using std::addressof;
        LOG("A new client connected, ip: {}, port: {}", ip, port);
    }
}

void on_client_msg(int fd)
{
    char    buf[512];
    ssize_t length    = -1;
    auto &&[ip, port] = get_info(fd);
    LOG("[Receive] From : {}:{}", ip, port);
    while ((length = recv(fd, buf, sizeof(buf), 0)) > 0)
    {
        LOG("\tMessage: {}", std::string(buf, length));
    }
    if (length == 0) {
        WARN("errno num: {}", errno);

        // LOG("Client disconnected");
        return;
    }
    else if (length == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            // No more data to read for now, do nothing
        }
        else {
            ERROR("Error occurred while receiving data from client: {} ", std::string(strerror(errno)));
        }
    }
}

int main(int argc, char **argv)
{
    int socket_opt = SOL_SOCKET | SOCK_NONBLOCK;
    int lfd        = socket(AF_INET, socket_opt, 0);
    check_perr(lfd, -1, "create socket");


    // set socket `lfd` reuse address before bind
    {
        int optval = 1;
        int ret    = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        check_perr(ret, -1, "Failed to set the `resue address` SO_RESUEADDR property of liten fd on initializing");
    }

    sockaddr_in server_addr;
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = 0;
    server_addr.sin_port        = htons(PORT);



    auto ret = bind(lfd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr));
    check_perr(ret, -1, "bind lfd with config domain");

    ret = listen(lfd, 128);
    check_perr(ret, -1, "on lfd initialze listening");
    {
        char buf[256];
        LOG("Sever listen on {}:{}", inet_ntop(AF_INET, &server_addr.sin_addr.s_addr, buf, 256), ntohs(server_addr.sin_port));
    }

    bool bRunning = true;


    while (bRunning) {

        on_new_client(lfd, client_fds, client_infos);

        // LOG("current client count: {}", client_fds.size());

        // Handle fds
        for (auto fd : client_fds) {
            on_client_msg(fd);
        }
    }



    return 0;
}

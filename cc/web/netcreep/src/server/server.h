#pragma once

#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/system/detail/error_code.hpp"
#include <boost/asio.hpp>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <format>
#include <iostream>
#include <memory>
#include <thread>

#include "../message.h"


namespace netcreep {
using namespace boost::asio;



class Session
    : public std::enable_shared_from_this<Session>
{
    ip::tcp::socket socket;
    std::thread     thread_heartbeat;

    Message msg;

  public:

    Session(ip::tcp::socket sock) : socket(std::move(sock)) {}
    ~Session() { thread_heartbeat.join(); }

    void start()
    {
        get_transformed_address();
        // do_read_header();
    }



    void do_read_header()
    {
        auto self(shared_from_this());
        auto buf = boost::asio::buffer(msg.data, Message::HEADER_LEN);

        socket.async_read_some(buf, [self](boost::system::error_code ec, std::size_t len) {
            if (!ec && self->msg.decoded_header()) {
                self->do_read_body();
            }
        });
    }

    void do_read_body()
    {
        auto self(shared_from_this());
    }

    void get_transformed_address()
    {
        auto self(shared_from_this());
        thread_heartbeat = std::thread([self]() {
            while (1) {
                auto addr = self->socket.remote_endpoint().address();
                std::cout << std::format("This session address: {}", addr.to_string());
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
    }
};

class Server
{
    ip::tcp::acceptor acceptor;

  public:
    Server(io_context &ctx, const ip::tcp::endpoint &endpoint)
        : acceptor(ctx, endpoint)
    {
    }

    void do_accept()
    {
        acceptor.async_accept([this](boost::system::error_code ec, ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket))->start();
            }
            do_accept();
        });
    }
};

} // namespace netcreep
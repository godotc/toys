#pragma once


#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/address.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/system/detail/error_code.hpp"
#include <boost/asio.hpp>
#include <cstdio>

#include "../message.h"

namespace netcreep {
using namespace boost::asio;



class Client
{
    ip::tcp::socket   cfd;
    ip::tcp::endpoint server_endpoint;
    Message           msg;


  public:
    Client(io_context &ctx, ip::tcp::endpoint &&server_endpoint)
        : cfd(ctx, ip::tcp::v4()), server_endpoint(std::move(server_endpoint))
    {
        connect();
    };

    void connect()
    {
        boost::system::error_code ec;

        cfd.async_connect(server_endpoint, [](const boost::system::error_code &ec) {
            if (ec) {
                throw std::runtime_error("Failed to connect to the remote end");
            }
        });

        // auto buf = boost::asio::buffer(msg.data, msg.HEADER_LEN);
        // cfd.async_read_some(buf, [&buf](auto ec, auto len) {
        //     printf("%s\n", buf.begin());
        // });
    }

    void send_something()
    {
    }
};
} // namespace netcreep
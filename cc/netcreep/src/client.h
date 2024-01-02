#pragma once


#include "boost/asio/buffer.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/system/detail/error_code.hpp"
#include <boost/asio.hpp>
#include <cstddef>
#include <memory>

namespace netcreep {
using namespace boost::asio;



class Client
{
    ip::tcp::socket cfd;


  public:
    Client(io_context &ctx)
        : cfd(ctx, ip::tcp::v4()){};

    void connect(std::string ip)
    {
        auto end = ip::tcp::endpoint(ip.c_str(), 9527);

        cfd.connect()
    }
};
} // namespace netcreep
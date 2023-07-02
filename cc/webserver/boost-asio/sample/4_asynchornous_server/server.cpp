#include "log.h"
#include <boost_pch.h>

#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/system/system_error.hpp>

#include "config.h"
#include <cstddef>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <thread>

using namespace boost::asio;
using socket_ptr = std::shared_ptr<ip::tcp::socket>;
using EC         = boost::system::error_code;

std::set<std::shared_ptr<std ::thread>> thread_set;

void session(const socket_ptr &sock)
{
    try {
        while (true) {
            char data[MAX_PACK_LEN];
            memset(data, '\0', MAX_PACK_LEN);
            EC ec;
            // size_t length = boost::asio::read(sock, boost::asio::buffer(data, MAX_PACK_LEN), ec);
            size_t length = sock->read_some(buffer(data, MAX_PACK_LEN), ec);
            if (ec == boost::asio::error::eof) {
                LOG("Connection closed by peer");
                break;
            }
            else if (ec) {
                throw boost::system::system_error(ec);
            }

            // std::string_view msg = std::vformat(fmt_str, std::make_format_args(args...));
            DEBUG("Receive from {} | Message: {}", sock->remote_endpoint().address().to_string(), data);

            sock->write_some(buffer(data, length));
        }
    }
    catch (std::exception &e) {
        LOG_ERROR("{}", e.what());
    }
}

// accept cnnection
void serve(io_context &ioc, unsigned short port)
{
    ip::tcp::acceptor acceptor(ioc, ip::tcp::endpoint(ip::tcp::v4(), port));

    while (true) {
        socket_ptr sock(new ip::tcp::socket(ioc));
        acceptor.accept(*sock);
        auto t = std::make_shared<std::thread>(session, sock);
        thread_set.insert(t);
    }
}



int main()
{
    LOG("srver listening on {}:{}", server_address, server_port);

    try {
        io_context ioc;
        serve(ioc, server_port);
        for (auto &t : thread_set) {
            t->join();
        }
    }
    catch (std::exception &e) {
        LOG_ERROR("{}", e.what());
    }
}

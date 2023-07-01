#include <boost_pch.h>

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/detail/error_code.hpp>

#include "config.h"
#include <cstddef>
#include <exception>
#include <iostream>



int main()
{
    using namespace boost::asio::ip;
    try {

        boost::asio::io_context   ioc;
        tcp::endpoint             remote_ep(address::from_string(server_address), server_port);
        tcp::socket               sock(ioc);
        boost::system::error_code EC = boost::asio::error::host_not_found;


        sock.connect(remote_ep, EC);
        if (EC.value() != 0) {
            LOG_ERROR("Faild to connect to server {} | {}", EC.value(), EC.message());
        }


        while (1) {
            std::cout << "Input message>";
            char request[MAX_PACK_LEN];
            std::cin.getline(request, MAX_PACK_LEN);

            size_t request_len = strlen(request);
            boost::asio::write(sock, boost::asio::buffer(request, request_len));

            char   rep[MAX_PACK_LEN];
            size_t reply_len = boost::asio::read(sock, boost::asio::buffer(rep, request_len));
            LOG("Reply: {}", rep);


            // sock.open(remote_ep.protocol(), EC);
            // if (EC.value() != 0) {
            //     ERROR("Faild to create sock {} | {}", EC.value(), EC.message());
            // }
            //
        }
    }
    catch (std::exception &e) {
        LOG_ERROR("{}", e.what());
    }
}

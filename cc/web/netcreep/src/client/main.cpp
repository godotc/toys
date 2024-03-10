#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <stacktrace>

#include "./client.h"

int main(int argc, char **argv)
{
    try {
        if (argc < 3) {
            std::cout << std::format("Usage: {} <ip> <port>\n", argv[0]);
        }

        printf("%s\n", std::format("{} {}", argv[1], argv[2]).c_str());

        using namespace boost::asio;
        io_context       io_context;
        auto             server_endpoint = ip::tcp::endpoint(ip::address::from_string(argv[1]), std::atoi(argv[2]));
        netcreep::Client client(io_context, std::move(server_endpoint));

        printf("Hello\n");
        io_context.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>

#include "./server.h"

int main(int argc, char **argv)
{
    try {
        if (argc < 2) {
            std::cout << std::format("Usage: {} <port>\n", argv[0]);
        }

        using namespace boost::asio;
        io_context        io_context;
        ip::tcp::endpoint endpoint(ip::tcp::v4(),
                                   std::atoi(argv[1]));

        netcreep::Server server(io_context, endpoint);

        server.do_accept();

        io_context.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

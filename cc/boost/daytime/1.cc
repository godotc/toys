#include <bits/types/time_t.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <cstddef>
#include <ctime>
#include <exception>
#include <iostream>



int main(int argc, char *argv[])
{
    using tcp = boost::asio::ip::tcp;

    if (argc != 2)
    {
        std::cerr << "Usage: client <host>" << std::endl;
        return 1;
    }

    try
    {
        boost::asio::io_context Context;
        tcp::resolver           Resolver(Context);

        tcp::resolver::results_type Endpoints = Resolver.resolve(argv[1], "daytime");
        tcp::socket                 ServerSocket(Context);

        boost::asio::connect(ServerSocket, Endpoints);

        while (1) {
            boost::array<char, 128>   buffer;
            boost::system::error_code error;

            size_t len = ServerSocket.read_some(boost::asio::buffer(buffer), error);
            if (error == boost::asio::error::eof) {
                throw boost::system::system_error(error);
            }
            else if (error) {
                throw boost::system::system_error(error);
            }

            std::cout.write(buffer.data(), len);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

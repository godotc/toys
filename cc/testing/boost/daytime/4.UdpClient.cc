#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/detail/array_fwd.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address_v6.hpp>
#include <boost/asio/ip/udp.hpp>
#include <cstddef>
#include <exception>
#include <iostream>
#include <sstream>
#include <sys/socket.h>



int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: client <host>" << std::endl;
        return 1;
    }
    using udp = boost::asio::ip::udp;

    try {
        boost::asio::io_context Ctx;

        udp::resolver Resolver(Ctx);
        udp::endpoint ReceiverEndpoints = *Resolver.resolve(udp::v4(), argv[1], "daytime").begin();

        udp::socket Socket(Ctx);
        Socket.open(udp::v4());

        boost::array<char, 1> sendBuf = {{'0'}};
        Socket.send_to(boost::asio::buffer(sendBuf), ReceiverEndpoints);

        udp::endpoint           SenderEndpoints;
        boost::array<char, 128> recvBuf;
        size_t                  len = Socket.receive_from(boost::asio::buffer(recvBuf), SenderEndpoints);

        std::cout.write(recvBuf.data(), len);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

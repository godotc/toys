#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include <stdexcept>
#include <system_error>


std::string makeDaytimeString()
{
    time_t now = time(nullptr);
    return std::ctime(&now);
}


int main()
{

    using tcp = boost::asio::ip::tcp;
    try {
        boost::asio::io_context Ctx;

        tcp::acceptor Acceptor(Ctx, tcp::endpoint(tcp::v4(), 13));

        for (;;) {
            tcp::socket socket(Ctx);
            Acceptor.accept(socket);

            auto msg = makeDaytimeString();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(msg), ignored_error);
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

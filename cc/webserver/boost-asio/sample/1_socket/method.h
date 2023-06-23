#include "boost_pch.h"


namespace uk {

using error_code = boost ::system::error_code;
using namespace boost::asio;
using namespace boost::asio::ip;

const auto     raw_server_ip_addrress = "127.0.0.1";
unsigned short port_num               = 9527;

inline int client_end_point()
{
    using namespace boost::asio;

    boost::system::error_code ec;

    auto ip = ip::address::from_string(raw_server_ip_addrress, ec);
    if (ec.value() != 0) {
        ERROR("Failer to parse IP addr, error code: {} | {}", ec.value(), ec.message());

        return ec.value();
    }

    ip::tcp::endpoint end_poin(ip, port_num);
}


inline int server_end_point()
{
    using namespace boost::asio;
    auto ip = ip::address_v6::any();

    ip::tcp::endpoint end_point(ip, port_num);

    return 0;
}

inline int create_tcp_socket()
{
    using namespace boost::asio;

    io_context      ioc;
    ip::tcp         protocol = ip::tcp::v4();
    ip::tcp::socket sock(ioc);

    boost::system::error_code ec;
    sock.open(protocol, ec);
    if (ec.value() != 0) {
        ERROR("Faile to open the socket , error code: {} | {}", ec.value(), ec.message());

        return ec.value();
    }

    return 0;
}

int inline create_acceptor_socket()
{
    using namespace boost::asio;
    using namespace boost::asio::ip;

    io_context ioc;
    error_code ec;
#if 0

    ip::tcp::endpoint ep(ip::address_v4::any(), port_num);

    tcp::acceptor acceptor(ioc, ep.protocol());
    acceptor.bind(ep, ec);
    if (ec.value() != 0) {
        ERROR("Faile to bind the socket , error code: {} | {}", ec.value(), ec.message());
        return ec.value();
    }

    auto protocl = tcp::v4();
    acceptor.open(protocl, ec);
    if (ec.value() != 0) {
        ERROR("Faile to open the socket , error code: {} | {}", ec.value(), ec.message());
        return ec.value();
    }

#else
    // New version of socket, bind address and port automatically

    ip::tcp::acceptor acceptor(ioc, ip::tcp::endpoint(ip::tcp::v4(), port_num));
#endif

    return 0;
}



int inline connect_to_server_end()
{
    const auto server_raw_ip = "127.0.0.1";

    try {
        ip::tcp::endpoint ep(ip::address::from_string(server_raw_ip), port_num);
        io_context        ioc;
        ip::tcp::socket   cfd(ioc, ep.protocol());

        cfd.connect(ep);
    }
    catch (boost::system::system_error &e) {
        ERROR("Connect to server {} | {}", e.code(), e.what());
        return e.code().value();
    }
    return 0;
}

int dns_connect_to_end()
{
    auto domain = "godot.link";
    auto port   = "9527";

    io_context               ioc;
    ip::tcp::resolver::query resolver_query(domain, port, ip::tcp::resolver::query::numeric_service);
    ip::tcp::resolver        resolver(ioc);


    try {
        ip::tcp::resolver::iterator ips = resolver.resolve(resolver_query);
        ip::tcp::socket             sock(ioc);
        boost::asio::connect(sock, ips);
    }
    catch (boost::system::system_error &e) {
        ERROR("Connect to server {} | {}", e.code(), e.what());
        return e.code().value();
    }
    return 0;
}


int accept_new_connection()
{
    const int BACKLOG_SIZE = 30;

    ip::tcp::endpoint ep(ip::address_v4::any(), port_num);
    io_context        ioc;

    try {
        ip::tcp::acceptor acceptor(ioc, ep.protocol());
        acceptor.bind(ep);
        acceptor.listen(BACKLOG_SIZE);
        ip::tcp::socket sock(ioc);
        acceptor.accept(sock);
    }
    catch (boost::system::system_error &e) {
        ERROR("Connect to server {} | {}", e.code(), e.what());
        return e.code().value();
    }
}

}; // namespace uk

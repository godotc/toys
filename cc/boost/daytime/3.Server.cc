#include <boost/asio.hpp>
#include <boost/asio/basic_socket_acceptor.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/system/detail/error_code.hpp>
#include <cstddef>
#include <ctime>
#include <exception>
#include <functional>
#include <iostream>
#include <system_error>


static auto makeDaytimeString()
{
    auto t = time(nullptr);
    return std::ctime(&t);
}

class TcpConnection : public boost::enable_shared_from_this<TcpConnection>
{
  public:
    using tcp     = boost::asio::ip::tcp;
    using pointer = boost::shared_ptr<TcpConnection>;

    static pointer New(boost::asio::io_context &io_context)
    {
        return pointer(new TcpConnection(io_context));
    }

    tcp::socket &Socket()
    {
        return m_Socket;
    }

    void Start()
    {
        m_Message = makeDaytimeString();

        boost::asio::async_write(m_Socket, boost::asio::buffer(m_Message),
                                 boost::bind(&TcpConnection::HanldeWrite,
                                             shared_from_this(),
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }

  private:
    TcpConnection(boost::asio::io_context &io_context) : m_Socket(io_context) {}

    void HanldeWrite(const boost::system::error_code /*&error*/, size_t /*bytes_transferred*/) {}


    tcp::socket m_Socket;
    std::string m_Message;
};


class TcpServer
{
    using tcp = boost::asio::ip::tcp;

  public:
    TcpServer(boost::asio::io_context &io_context)
        : m_Context(io_context),
          m_Acceptor(io_context, tcp::endpoint(tcp::v4(), 13))
    {
        StartAccept();
    }

    void StartAccept()
    {
        TcpConnection::pointer NewConnection = TcpConnection::New(m_Context);
        m_Acceptor.async_accept(NewConnection->Socket(),
                                boost::bind(&::TcpServer::HanldeAccept, this, NewConnection,
                                            boost::asio::placeholders::error));
    }

    void HanldeAccept(const TcpConnection::pointer &NewConnection, const boost::system::error_code &error)
    {
        if (!error) {
            NewConnection->Start();
        }

        StartAccept();
    }

  private:
    boost::asio::io_context &m_Context;
    tcp::acceptor            m_Acceptor;
};


int main()
{
    boost::asio::io_context Ctx;
    try {

        TcpServer Server(Ctx);
        Ctx.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

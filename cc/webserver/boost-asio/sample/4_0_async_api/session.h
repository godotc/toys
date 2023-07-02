#pragma once

#include "boost/asio/registered_buffer.hpp"
#include "boost/system/detail/error_code.hpp"
#include "log.h"
#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <optional>
#include <queue>
#include <string.h>
#include <system_error>
#include <vcruntime.h>



using namespace std;
using namespace boost;

const inline int RECVSIZE = 1024;

class MsgNode
{
  public:
    MsgNode(const char *msg, int len)
    {
        m_TotalLen = len;
        m_Msg      = new char[RECVSIZE];
        memcpy(m_Msg, msg, len);
    }

    MsgNode(int len)
    {
        m_TotalLen = len;
        m_Msg      = new char[RECVSIZE];
    }

    virtual ~MsgNode() { delete m_Msg; }

  public:
    int   m_TotalLen;
    int   m_CurLen = 0;
    char *m_Msg;
};

class Session
{
    using EC = boost::system::error_code;

  public:
    Session(std::shared_ptr<asio::ip::tcp::socket> socket);

    void Connect(const asio::ip::tcp::endpoint &endpoint);

    // Cannot figure out the return value(bytes has been transffered), maybe cause multiply send;
    void WriteCallbackErr(const EC &error_code, size_t bytes_transfered, std::shared_ptr<MsgNode> Node)
    {
        if (bytes_transfered + Node->m_CurLen < Node->m_TotalLen) {
            m_SendNode->m_CurLen += bytes_transfered;
            m_Socket->async_write_some(
                asio::buffer(m_SendNode->m_Msg + m_SendNode->m_CurLen, m_SendNode->m_TotalLen - m_SendNode->m_CurLen),
                std::bind(&Session::WriteCallbackErr, this, std::placeholders::_1, std::placeholders::_2, m_SendNode));
        }
        else {
        }
    }

    void WriteToSocketErr(const std::string buf)
    {
        m_SendNode = make_shared<MsgNode>(buf.c_str(), buf.length());
        this->m_Socket->async_write_some(
            asio::buffer(m_SendNode->m_Msg, m_SendNode->m_TotalLen),
            std::bind(&Session::WriteCallbackErr, this, std::placeholders::_1, std::placeholders::_2, m_SendNode));
    }


    // Queue to prove the send_data's time sequence(时序性)
    // Need to reduce the call tims of read call back
    void WriteCallback(const EC &error_code, size_t bytes_transfered)
    {
        if (error_code.value() != 0) {
            LOG_ERROR("Error code: {} | {}", error_code.value(), error_code.message());
        }

        auto &send_data = m_SendQueue.front();
        send_data->m_CurLen += bytes_transfered;
        if (send_data->m_CurLen < send_data->m_TotalLen) {
            this->m_Socket->async_write_some(
                asio::buffer(send_data->m_Msg + send_data->m_CurLen, send_data->m_TotalLen - send_data->m_CurLen),
                std::bind(&Session::WriteCallback, this, std::placeholders::_1, std::placeholders::_2));
            return;
        }


        m_SendQueue.pop();

        if (m_SendQueue.empty()) {
            m_SendPending = false;
        }

        if (!m_SendQueue.empty()) {
            m_SendPending   = false;
            auto &send_data = m_SendQueue.front();
            send_data->m_CurLen += bytes_transfered;
            this->m_Socket->async_write_some(
                asio::buffer(send_data->m_Msg + send_data->m_CurLen, send_data->m_TotalLen - send_data->m_CurLen),
                std::bind(&Session::WriteCallback, this, std::placeholders::_1, std::placeholders::_2));
        }
    }

    void WriteToSocket(const std::string buf)
    {
        m_SendQueue.emplace(new MsgNode(buf.c_str(), buf.length()));
        if (m_SendPending)
            return;

        this->m_Socket->async_write_some(
            asio::buffer(m_SendNode->m_Msg, m_SendNode->m_TotalLen),
            std::bind(&Session::WriteAllCallback, this, std::placeholders::_1, std::placeholders::_2));
        m_SendPending = true;
    }


    //  do not mix the async_write_some and async_send
    void WriteAllCallback(const EC &error_code, size_t bytes_transfered)
    {
        if (error_code.value() != 0) {
            LOG_ERROR("Error code: {} | {}", error_code.value(), error_code.message());
            return;
        }

        // callback occur when data hase send complete

        m_SendQueue.pop();

        if (m_SendQueue.empty()) {
            m_SendPending = false;
        }

        if (!m_SendQueue.empty()) {
            m_SendPending   = false;
            auto &send_data = m_SendQueue.front();
            send_data->m_CurLen += bytes_transfered;
            this->m_Socket->async_write_some(
                asio::buffer(send_data->m_Msg + send_data->m_CurLen, send_data->m_TotalLen - send_data->m_CurLen),
                std::bind(&Session::WriteAllCallback, this, std::placeholders::_1, std::placeholders::_2));
        }
    }

    void WriteAllToSocket(const std::string buf)
    {
        m_SendQueue.emplace(new MsgNode(buf.c_str(), buf.length()));
        if (m_SendPending)
            return;

        this->m_Socket->async_send(
            asio::buffer(m_SendNode->m_Msg, m_SendNode->m_TotalLen),
            std::bind(&Session::WriteAllCallback, this, std::placeholders::_1, std::placeholders::_2));
        m_SendPending = true;
    }

  private:
    std::queue<std::shared_ptr<MsgNode>>   m_SendQueue;
    std::shared_ptr<asio::ip::tcp::socket> m_Socket;
    std::shared_ptr<MsgNode>               m_SendNode;
    bool                                   m_SendPending = false;

  public:

    Session(std::shared_ptr<asio::ip::tcp::socket> socket, int total_len);

    void ReadCallBack(const EC &error_code, size_t bytes_received)
    {
        m_ReceiveNode->m_CurLen += bytes_received;
        if (m_ReceiveNode->m_CurLen < m_ReceiveNode->m_TotalLen) {
            m_Socket->async_read_some(
                asio::buffer(m_ReceiveNode->m_Msg, m_ReceiveNode->m_TotalLen),
                std::bind(&Session::ReadCallBack, this, placeholders::_1, placeholders::_2));
            return;
        }

        m_ReceivePending = false;
        m_ReceiveNode    = nullptr;
    }

    void ReadFromSocket()
    {
        if (m_ReceivePending)
            return;
        m_ReceiveNode = make_shared<MsgNode>(RECVSIZE);
        m_Socket->async_read_some(
            asio::buffer(m_ReceiveNode->m_Msg, m_ReceiveNode->m_TotalLen),
            std::bind(&Session::ReadCallBack, this, placeholders::_1, placeholders::_2));
    }

    //.... and so on for async_receive


  private:
    // std::shared_ptr<asio::ip::tcp::socket> m_Socket;
    std::shared_ptr<MsgNode> m_ReceiveNode;
    bool                     m_ReceivePending = false;
};
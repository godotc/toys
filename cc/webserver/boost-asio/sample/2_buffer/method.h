#include <boost/asio/buffer.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost_pch.h>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace uk {

using namespace boost;

inline void use_const_buffer()
{
    std::string                     a_buf = "hello world";
    asio::const_buffer              buffer(a_buf.c_str(), a_buf.size());
    std::vector<asio::const_buffer> buffer_seqs;
    buffer_seqs.push_back(buffer);
}

inline void use_buffer_str()
{
    asio::const_buffers_1 output_buf = asio::buffer("hello world");
}

inline void use_buffer_aray()
{
    const size_t BUF_SIZE_BYTES = 20;
    auto         buf            = std::unique_ptr<char[]>(new char[BUF_SIZE_BYTES]);
    auto         input_buf      = asio::buffer(static_cast<void *>(buf.get()), BUF_SIZE_BYTES);
}


auto write_to_socket(asio::ip::tcp::socket &sock)
{
    using namespace std;
    auto buf                 = "heelo wd"s;
    auto total_bytes_written = 0;

    while (total_bytes_written != buf.size()) {
        auto const_buf = asio::buffer(buf.c_str() + total_bytes_written, buf.size());
        auto n         = sock.write_some(const_buf);
        total_bytes_written += n;
    }
}



} // namespace uk

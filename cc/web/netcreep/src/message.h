#pragma once
#include <boost/asio.hpp>

namespace netcreep {
using namespace boost::asio;


struct Message {
    enum
    {
        HEADER_LEN   = 4,
        MAX_BODY_LEN = 512
    };
    char        data[HEADER_LEN + MAX_BODY_LEN];
    std::size_t body_len;

    bool decoded_header()
    {
        char header[HEADER_LEN + 1] = "";
        std::strncat(header, data, HEADER_LEN);
        body_len = std::atoi(header);
        if (body_len > MAX_BODY_LEN)
        {
            body_len = 0;
            return false;
        }
        return true;
    }

  public:
    Message() : body_len(0) {}
};
} // namespace netcreep
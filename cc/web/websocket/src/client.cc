#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const std::string host = "example.com";
const std::string port = "80";
const std::string path = "/websocket";

int cfd;

struct addrinfo hints, *res;

int main()
{
    // Connect to server
    {
        std::memset(&hints, 0, sizeof hints);
        hints.ai_family   = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0) {
            std::cerr << "Failed to get address info." << std::endl;
            return 1;
        }

        cfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (cfd == -1) {
            std::cerr << "Failed to create socket." << std::endl;
            return 1;
        }

        if (connect(cfd, res->ai_addr, res->ai_addrlen) == -1) {
            std::cerr << "Failed to connect to the server." << std::endl;
            return 1;
        }

        freeaddrinfo(res);
    }


    // Upgrade request
    {
        std::string handshake_request = "GET " + path + " HTTP/1.1\r\n"
                                                        "Host: " +
                                        host + "\r\n"
                                               "Upgrade: websocket\r\n"
                                               "Connection: Upgrade\r\n"
                                               "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
                                               "Sec-WebSocket-Version: 13\r\n"
                                               "\r\n";

        if (send(cfd, handshake_request.c_str(), handshake_request.length(), 0) == -1) {
            std::cerr << "Failed to send handshake request." << std::endl;
            return 1;
        }

        char        response_buffer[4096];
        std::string handshake_response;

        int bytes_received = recv(cfd, response_buffer, sizeof(response_buffer) - 1, 0);
        if (bytes_received == -1) {
            std::cerr << "Failed to receive handshake response." << std::endl;
            return 1;
        }

        handshake_response = std::string(response_buffer, bytes_received);

        // Check if the handshake was successful
        if (handshake_response.find(" 101 Switching Protocols\r\n") == std::string::npos) {
            std::cerr << "Handshake failed." << std::endl;
            return 1;
        }
    }

    // Send a msg
    {
        std::string message = "Hello, server!";

        // Create the WebSocket frame
        std::string frame;
        frame += '\x81';                              // FIN bit + Text frame opcode
        frame += static_cast<char>(message.length()); // Payload length
        frame += message;                             // Payload data

        // Send the WebSocket frame
        if (send(cfd, frame.c_str(), frame.length(), 0) == -1) {
            std::cerr << "Failed to send WebSocket frame." << std::endl;
            return 1;
        }
    }
}

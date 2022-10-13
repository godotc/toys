#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <cstdlib>
#include <iostream>
#include <linux/close_range.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>

#include "log.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/sha.h>

#define PORT 9527
#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

inline void f_err(int no_expect, int ret, const char *msg)
{
    if (no_expect == ret)
        perror(msg);
}
inline bool f_lerr(int upper_bound, int ret, const char *msg)
{
    if (ret <= upper_bound)
    {
        perror(msg);
        return 1;
    }
    return 0;
}

inline void invert_string(char *str, int len)
{
    int l = 0, r = len - 1;
    while (l < r)
        std::swap(str[l++], str[r--]);
}

inline void umask(char *data, int len, char *mask)
{
    for (int i = 0; i < len; ++i)
        *(data + i) ^= *(mask + (i % 4));
}

int base64_encode(char *in_str, size_t in_len, char *out_str)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    size_t size = 0;

    if (in_str == NULL || out_str == NULL)
        return -1;

    b64 = BIO_new(BIO_f_base64()); // encode
    bio = BIO_new(BIO_s_mem());    // io port
    bio = BIO_push(b64, bio);

    BIO_write(bio, in_str, in_len);
    BIO_flush(bio);

    BIO_get_mem_ptr(bio, &bptr);
    memcpy(out_str, bptr->data, bptr->length);

    out_str[bptr->length - 1] = '\0';
    size = bptr->length;

    BIO_free_all(bio);
    return size;
}

class WebSocketServer
{
  private:
    int m_lfd;

    struct frame_head
    {
        char fin;
        char opcode;
        char mask;
        unsigned long long payload_length;
        char masking_key[4];
    };

  public:
    void Init()
    {
        init_server(PORT, 20);

        struct sockaddr_in cli_addr;
        socklen_t cli_addr_len = sizeof(cli_addr);
        int cfd = accept(m_lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);

        shakehands(cfd);

        while (1)
        {
            frame_head head;
            int rul = recv_frame_head(cfd, &head);
            if (rul < 0)
                break;
            log_debug("fin=%d\nopcode=0x%X\nmask=%d\npayload_len=%llu\n", head.fin, head.opcode, head.mask,
                      head.payload_length);

            // echo head
            send_frame_head(cfd, &head);

            // read payload data
            char payload_data[BUFSIZ] = {};
            int size = 0;

            do
            {
                int rul;
                rul = read(cfd, payload_data, BUFSIZ);
                if (rul < 0)
                    break;
                size += rul;

                umask(payload_data, size, head.masking_key);
                log_debug("receive:%s", payload_data);

                // echo data
                if (write(cfd, payload_data, rul) <= 0)
                    break;
            } while (size < head.payload_length);

            printf("\n--------------------------\n");
        }

        close(cfd);
        close(m_lfd);
    }

  private:
    // create lfd, bind and listen
    void init_server(short port, int queue)
    {

        int lfd = socket(AF_INET, SOCK_STREAM, 0);
        f_err(-1, lfd, "socket");

        // set addr resuse
        int opt = 1;
        setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        int ret = bind(lfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
        f_err(-1, ret, "bind");
        log_trace("Bind succes");

        ret = listen(lfd, 128);
        f_err(-1, ret, "listen");
        log_debug("Listen on port %d", server_addr.sin_port);

        m_lfd = lfd;
    }

    /**
     * @brief read_line
     * Get a str of one line, return the next line begin offset
     */
    int read_line(char *allbuf, int offset, char *linebuf)
    {
        int len = strlen(allbuf);
        for (; offset < len; ++offset)
        {
            if (allbuf[offset] == '\r' && allbuf[offset + 1] == '\n')
                return offset + 2;
            else
                *(linebuf++) = allbuf[offset];
        }
        return -1;
    }

    /**
     * @brief shakehands
     * 负责处理新客户端的连接，接收客户端http格式的请求，从中获得Sec-WebSocket-Key
     * 对应的值，与魔法字符串 258EAFA5-E914-47DA-95CA-C5AB0DC85B11 进行连接后进行
     * sha1 hash，再将结果（sha1的直接结果，不是转化为字符串后的结果）进行Base64编码。
     * 最后构造响应头部，发送响应，与客户端建立websocket连接。
     */
    void shakehands(int cfd)
    {
        int offset = 0;
        char req_buf[BUFSIZ]; // all request data
        char linebuf[256];

        char sec_accept[32];                        // Sec-WebSocket-Accept
        char sha1_data[SHA_DIGEST_LENGTH + 1] = {}; // sha1 data
        char resp_head[BUFSIZ] = {};                // response header buffer

        if ((read(cfd, req_buf, BUFSIZ) <= 0))
            perror("read");

        log_debug("request:");
        log_debug("%s", req_buf);

        do
        {
            memset(linebuf, 0, sizeof(linebuf));
            offset = read_line(req_buf, offset, linebuf);
            printf(">> %s \n", linebuf);

            // Find ws key, then send response break
            if (strstr(linebuf, "Sec-WebSocket-Key") != NULL)
            {
                strcat(linebuf, GUID);
                log_debug("key:%s\nlen=%d\n", linebuf + 19, strlen(linebuf + 19));

                SHA1((unsigned char *)&linebuf + 19, strlen(linebuf + 19), (unsigned char *)&sha1_data);
                log_debug("sha1:%s\n", sha1_data);

                // Encoding: in str, size , out str
                base64_encode(sha1_data, strlen(sha1_data), sec_accept);
                log_debug("base64:%s\n", sec_accept);

                // write the response
                sprintf(resp_head,
                        "HTTP/1.1 101 Switching Protocols\r\n"
                        "Upgrade: websocket\r\n"
                        "Connection: Upgrade\r\n"
                        "Sec-WebSocket-Accept: %s\r\n"
                        "\r\n",
                        sec_accept);

                log_debug("response:");
                log_debug("%s", resp_head);
                if (write(cfd, resp_head, strlen(resp_head)) < 0)
                    perror("write");

                break;
            }

        } while ((req_buf[offset] != '\r' || req_buf[offset + 1] != '\n') && offset != -1);
    }

    int recv_frame_head(int fd, frame_head *head)
    {
        char a_byte;
        /*read fin and op code*/
        if (read(fd, &a_byte, 1) <= 0)
        {
            perror("read fin");
            return -1;
        }
        head->fin = (a_byte & 0x80) == 0x80;
        head->opcode = a_byte & 0x0F;
        if (read(fd, &a_byte, 1) <= 0)
        {
            perror("read mask");
            return -1;
        }
        head->mask = (a_byte & 0x80) == 0X80;

        /*get payload length*/
        head->payload_length = a_byte & 0x7F;

        if (head->payload_length == 126)
        {
            char extern_len[2];
            if (read(fd, extern_len, 2) <= 0)
            {
                perror("read extern_len");
                return -1;
            }
            head->payload_length = (extern_len[0] & 0xFF) << 8 | (extern_len[1] & 0xFF);
        }
        else if (head->payload_length == 127)
        {
            char extern_len[8];
            if (read(fd, extern_len, 8) <= 0)
            {
                perror("read extern_len");
                return -1;
            }
            invert_string(extern_len, 8);
            memcpy(&(head->payload_length), extern_len, 8);
        }

        /*read masking-key*/
        if (read(fd, head->masking_key, 4) <= 0)
        {
            perror("read masking-key");
            return -1;
        }

        return 0;
    }

    int send_frame_head(int fd, frame_head *head)
    {
        char *resp_head;
        int head_len = 0;

        if (head->payload_length < 126)
        {
            resp_head = (char *)malloc(2);
            resp_head[0] = 0x81;
            resp_head[1] = head->payload_length;
            head_len = 2;
        }
        else if (head->payload_length < 0XFFFF)
        {
            resp_head = (char *)malloc(4);
            resp_head[0] = 0x81;
            resp_head[1] = 126;
            resp_head[2] = (head->payload_length >> 8 & 0xFF);
            resp_head[3] = (head->payload_length & 0xFF);
            head_len = 4;
        }
        else
        {
            resp_head = (char *)malloc(12);
            resp_head[0] = 0x81;
            resp_head[1] = 127;
            memcpy(resp_head + 2, (char *)head->payload_length, sizeof(unsigned long long));
            head_len = 12;
        }

        if (write(fd, resp_head, head_len) <= 0)
        {
            perror("write head");
            return -1;
        }

        free(resp_head);
        return 0;
    }
};

/*
buffer_compat.h       dns_compat.h    event_struct.h    rpc_compat.h  util.h
bufferevent_compat.h  dns.h           http_compat.h     rpc.h         visibility.h
bufferevent.h         dns_struct.h    http.h            rpc_struct.h
bufferevent_ssl.h     event_compat.h  http_struct.h     tag_compat.h
bufferevent_struct.h  event-config.h  keyvalq_struct.h  tag.h
buffer.h              event.h         listener.h        thread.h
*/

#include <arpa/inet.h>
#include <bits/getopt_core.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void           dump_request_cb(struct evhttp_request *req, void *arg);
static void           send_document_cb(struct evhttp_request *req, void *arg);
static struct options parse_opts(int argc, char **argv);


struct options
{
    int port;
    int iocp;
    int verbose;
    int max_body_size;

    int         unlink;
    const char *unixsock;
    const char *bind;
    const char *docrrot;
};

int main(int argc, char *argv[])
{
    struct event_config   *cfg  = NULL;
    struct event_base     *base = NULL;
    struct evhttp         *http = NULL;
    struct evconnlistener *lev  = NULL;
    struct event          *term = NULL;
    struct options         o    = parse_opts(argc, argv);
    int                    ret  = 0;

#ifdef linux
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        ret = 1;
        goto err;
    }
#endif

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    // read env like in regress ¿?)
    if (o.verbose || getenv("EVENT_DEBUG_LOGGING_ALL"))
        event_enable_debug_logging(EVENT_DBG_ALL);

    base = event_base_new_with_config(cfg);
    if (!base)
    {
        fprintf(stderr, "Couldn't create an event_base: exiting\n");
        ret = 1;
    }
    event_config_free(cfg);
    cfg = NULL;

    // Create a new evhttp object to handle requests ev
    http = evhttp_new(base);
    if (!http)
    {
        fprintf(stderr, "Couldn't create evhttp. Exiting. \n");
        ret = 1;
    }

    // /dump URL dump all requests to stdout and say 200 OK
    evhttp_set_cb(http, "/dump", dump_request_cb, NULL);

    evhttp_set_gencb(http, send_document_cb, &o);
    if (o.max_body_size)
        evhttp_set_max_body_size(http, o.max_body_size);

err:
    if (cfg)
        event_config_free(cfg);
    if (http)
        event_config_free(http);
    if (term)
        event_config_free(term);
    if (base)
        event_config_free(base);

    return ret;
}

static void
dump_request_cb(struct evhttp_request *req, void *arg)
{
    const char       *cmd_type;
    struct evkeyvalq *headers;
    struct evkeyval  *header;
    struct evbuffer  *buf;

    int request_type = evhttp_request_get_command(req);

    switch (request_type)
    {
        case EVHTTP_REQ_GET:
            cmd_type = "GET";
            break;
        case EVHTTP_REQ_POST:
            cmd_type = "POST";
            break;
        case EVHTTP_REQ_HEAD:
            cmd_type = "HEAD";
            break;
        case EVHTTP_REQ_PUT:
            cmd_type = "PUT";
            break;
        case EVHTTP_REQ_DELETE:
            cmd_type = "DELETE";
            break;
        case EVHTTP_REQ_OPTIONS:
            cmd_type = "OPTIONS";
            break;
        case EVHTTP_REQ_TRACE:
            cmd_type = "TRACE";
            break;
        case EVHTTP_REQ_CONNECT:
            cmd_type = "CONNECT";
            break;
        case EVHTTP_REQ_PATCH:
            cmd_type = "PATCH";
            break;
        default:
            cmd_type = "unknown";
            break;
    }

    printf("Received a %s request for %s \nHeaders: \n",
           cmd_type, evhttp_request_get_uri(req));

    headers = evhttp_request_get_input_headers(req);
    for (header = headers->tqh_first;
         header;
         header = header->next.tqe_next)
    {
        printf("  %s: %s \n", header->key, header->value);
    }

    buf = evhttp_request_get_input_buffer(req);

    puts("Input data: <<<");
    while (evbuffer_get_length(buf)) {
        int  n;
        char cbuf[128];
        n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
        if (n > 0)
            (void)fwrite(cbuf, 1, n, stdout);
    }
    puts(">>>");

    evhttp_send_reply(req, 200, "OK", NULL);
}

static void
send_document_cb(struct evhttp_request *req, void *arg)
{
    fprintf(stdout, "send_document_cb invoke!\n");
}

static struct options
parse_opts(int argc, char **argv)
{
    struct options o;
    int            opt;

    memset(&o, 0, sizeof(o));

    while ((opt = getopt(argc, argv, "hp:U:m:uIvH:")) != -1)
    {
        switch (opt) {
            case 'p':
                o.port = atoi(optarg);
                break;
            case 'U':
                o.unixsock = optarg;
                break;
            case 'u':
                o.unlink = 1;
                break;
            case 'I':
                o.iocp = 1;
                break;
            case 'm':
                o.max_body_size = atoi(optarg);
                break;
            case 'v':
                ++o.verbose;
                break;
            case 'H':
                o.bind = optarg;
                break;
            case 'h':
                print_usage(stdout, argv[0], 0);
                break;
            default:
                fprintf(stderr, "Unknown option %c\n", opt);
                break;
        }
    }

    if (optind >= argc || (argc - optind) > 1)
    {
        print_usage(stdout, argv[0], 1);
    }
    o.docrrot = argv[optind];

    return o;
}

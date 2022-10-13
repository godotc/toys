#include "log.h"
#include "sock_server.hpp"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <pthread.h>

pthread_mutex_t LOG_MUTEX;

void log_lock(bool is_unlocked, void *udata)
{
    pthread_mutex_t *LOCK = (pthread_mutex_t *)(udata);
    if (is_unlocked)
        pthread_mutex_lock(LOCK);
    else
        pthread_mutex_unlock(LOCK);
}

int main()
{
    log_set_level(0);
    log_set_quiet(0);
    pthread_mutex_init(&LOG_MUTEX, NULL);
    log_set_lock(log_lock, &LOG_MUTEX);

    WebSocketServer server;
    server.Init();

    pthread_mutex_destroy(&LOG_MUTEX);
    return 0;
}


#include <pthread.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <stdexcept>



#define LL_INSERT(item, list)  \
    do {                       \
        /* head insert */      \
        item->prev = nullptr;  \
        item->next = list;     \
        if (list != nullptr)   \
            list->prev = item; \
        list = item;           \
    } while (0)


#define LL_REMOVE(item, list)              \
    do {                                   \
        if (item->prev != NULL)            \
            item->prev->next = item->next; \
        if (item->next != NULL)            \
            item->next->prev = item->prev; \
        if (list == item)                  \
            list = item->next;             \
        item->prev = item->next = NULL;    \
    } while (0)


struct NTASK
{
    void(task_func)(void *arg);
    void         *user_data;
    struct NTASK *prev;
    struct NTASK *next;
};

struct NWORKER
{
    pthread_t m_pid;
    int       terminate{0};

    struct NMANAGER *pool;

    struct NWORKER *prev;
    struct NWORKER *next;
};

struct NMANAGER
{
    struct NWORKER *workers;
    struct NTASK   *tasks;

    pthread_mutex_t mtx;
    pthread_cond_t  cond;
} threadpool_t;



// static callback (entry)
// 用于函数只在本文件有效
static void *thread_callback(void *arg)
{
    auto worker = (NWORKER *)arg;

    while (1) {
        pthread_mutex_lock(&worker->pool->mtx);
        while (worker->pool->tasks == NULL) {
            pthread_cond_wait(&worker->pool->cond, &worker->pool->mtx);
        }

        if (worker->terminate) {
            pthread_mutex_unlock(&worker->pool->mtx);
            exit(-1);
        }

        struct NTASK *task = worker->pool->tasks;
        if (task) {
            LL_REMOVE(task, worker->pool->tasks);
        }

        pthread_mutex_unlock(&worker->pool->mtx);

        task->task_func(task);
    }
}


// create
int threadPoolCreate(struct NMANAGER *pool, int nthread)
{
    if (pool == nullptr) return -1;
    if (nthread < 1) nthread = 1;

    memset(pool, 0, sizeof(NMANAGER));

    pthread_mutex_t balnk_mtx = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&pool->mtx, &balnk_mtx, sizeof(balnk_mtx));

    pthread_cond_t bland_cond = PTHREAD_COND_INITIALIZER;
    memcpy(&pool->cond, &bland_cond, sizeof(bland_cond));


    for (int i = 0; i < nthread; ++i) {

        struct NWORKER *worker = new (NWORKER);
        if (worker == NULL) {
            perror("new/malloc new thread");
            exit(-1);
        }
        memset(worker, 0, sizeof(NWORKER));
        worker->pool = pool;


        int ret = pthread_create(&worker->m_pid, NULL, thread_callback, worker);

        if (ret) {
            perror("pthread_create error");
            free(worker);
            exit(-1);
        }

        LL_INSERT(worker, pool->workers);
    }

    return 0;
}

// destory
int threadPoolDestory()
{
}

// push
int threadPoolPush(struct NMANAGER *pool, struct NTASK *task)
{
    pthread_mutex_lock(&pool->mtx);

    LL_INSERT(task, pool->tasks);
    pthread_cond_signal(&pool->cond);

    pthread_mutex_unlock(&pool->mtx);

    return 0;
}

// pop



int main()
{
    NMANAGER *p_pool;

    int nthread = 20;
    threadPoolCreate(p_pool, nthread);


    return 0;
}

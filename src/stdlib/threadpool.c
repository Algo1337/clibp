#include "../../headers/threadpool.h"

threadpool_t create_pool(int MAX_THREADS, int MAX_RUNNING_THREADS) {
    if(!MAX_THREADS || !MAX_RUNNING_THREADS)
        return NULL;

    threadpool_t pool = (threadpool_t)malloc(sizeof(threadpool_T));

    pool->max_threads = MAX_THREADS;
    pool->max_running_threads = MAX_RUNNING_THREADS;
    pool->threads = new_arr(NULL, 0);

    return pool;
}

int tp_Append(threadpool_t pool, thread_t t) {
    if(!pool || !pool->threads || !t)
        return -1;

    arr_Append(pool->threads, t);
    return pool->threads->idx;
}

void start_pool(threadpool_t pool) {
    if(!pool || pool->running || pool->threads->idx <= 0)
        return;

    pool->running = 1;
    pthread_create(&pool->id, NULL, (void *)tp_run_threads, (void *)pool);
}

void tp_run_threads(threadpool_t pool) {
    if(!pool && !pool->running)
        return;

    while(pool->running) {
        for(int i = 0; i < pool->threads->idx; i++) {
            if(!pool->threads->arr[i])
                break;

            thread_t thread = (thread_t)pool->threads->arr[i];
            if(thread->keep_up && thread->running)
                continue;

            if(thread->finished) {
                // Thread finished execution
                arr_Remove(pool->threads, i, thread_Destruct);
                pool->running_threads--;
                break;
            } else if((thread->running && !thread->finished) || pool->running_threads >= pool->max_running_threads) {
                // Running thread or current running threads is over max running threads
                usleep(30000);
                continue;
            }

            // Run thread and increment running_threads
            thread_execute(thread);
            pool->running_threads++;
        }

        if(pool->running_threads == 0 && pool->threads->idx == 0)
            break;

        sleep(1);
    }

    pool->running = 0;
}
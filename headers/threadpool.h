#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CLIBP
#include "clibp.h"
#include "thread.h"

typedef arr_t threads_t;

typedef struct threadpool_T {
    threads_t     threads;
    int           running;

    int           running_threads;
    int           max_threads;
    int           max_running_threads;
    pthread_t     id;
} threadpool_T;

typedef threadpool_T *threadpool_t;


// Create threadpool_t Instanse
threadpool_t create_pool(int MAX_THREADS, int MAX_RUNNING_THREADS);

// Append thread to pool
int tp_Append(threadpool_t pool, thread_t t);

// Start thread pool
void start_pool(threadpool_t pool);

// Threaded pool iteration function
void tp_run_threads(threadpool_t pool);
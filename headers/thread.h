#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct thread_T {
    int         tid;

    void        *handler;
    void        **args;
    void        *return_var;
    pthread_t   id;

    int         running;
    int         finished;
    int         keep_up;
    void        *base;
    int         base_arg;
} thread_T;

typedef thread_T *thread_t;

// Create thread Instanse and execute thread
thread_t create_n_execute_thread(void *handler, void **args, int base_arg);

// Create a thread_t Instanse
thread_t create_thread(void *handler, void **args, int base_arg);

// Execute thread
int thread_execute(thread_t t);

// Wait for thread execution to signal or complete
int thread_wait(thread_t t);

// Complete thread toggle
int thread_complete(thread_t t);

// Exit thread
int thread_exit(thread_t t);

// Destruct thread struct
void thread_Destruct(thread_t t);
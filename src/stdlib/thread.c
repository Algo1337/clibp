#include "../../headers/thread.h"

thread_t create_n_execute_thread(void *handler, void *args, int base_arg) {
    thread_t t = create_thread(handler, args, base_arg);

    thread_execute(t);
    return t;
}

thread_t create_thread(void *handler, void *args, int base_arg) {
    thread_t t = (thread_t)malloc(sizeof(thread_T));
    memset(t, '\0', sizeof(thread_T));

    t->handler = handler;
    t->args = args;
    t->base_arg = base_arg;

    return t;
}

int thread_execute(thread_t t) {
    if(!t || !t->handler)
        return 0;

    t->running = 1;
    if(t->base_arg) {
        pthread_create(&t->id, NULL, (void *)t->handler, (void *)t);
        return 1;
    } else if(t->args) {
        pthread_create(&t->id, NULL, (void *)t->handler, (void *)t->args);
        return 1;
    } 
    
    pthread_create(&t->id, NULL, (void *)t->handler, NULL);
    return 1;
}

int thread_wait(thread_t t) {
    if(!t || !t->running)
        return -1;

    (void)(t->return_var ? pthread_join(t->id, t->return_var) : pthread_join(t->id, NULL));
    t->running = 0;
    t->finished = 1;
    return 1;
}

int thread_complete(thread_t t) {
    if(!t || !t->running)
        return 0;

    t->running = 0;
    t->finished = 1;
    return 1;
}

int thread_exit(thread_t t) {
    if(!t || (!t->running || !t->finished))
        return -1;

    pthread_cancel(t->id);
    pthread_join(t->id, t->return_var);

    t->running = 0;
    t->finished = 1;
    return 1;
}

void thread_Destruct(thread_t t) {
    if(!t)
        return;

    free(t);
}
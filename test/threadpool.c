#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <threadpool.h>

void test_fn(thread_t t)  {
	for(int i = 0; i < 10; i++) {
		usleep(5000);
	}

	thread_complete(t);
}

int main() {
	threadpool_t pool = create_pool(100, 60);
	if(!pool) {
		printf("[ - ] Unable to create threadpool!\n");
		return 1;
	}

	for(int i = 0; i < 80; i++)
		if(tp_Append(pool, create_thread(test_fn, NULL, 1)) < 0)
			printf("[ - ] Error, Unable to add thread @ %d!\n", i);

	if(pool->threads->idx == 0) {
		printf("[ - ] Error, No threads to start pool!\n");
		return 1;
	}

	start_pool(pool);
	int ticks = 0;
	while(pool->running) {
		printf("[ Threadpool: %d ] %d/%d threads is still running....!\r", ticks, pool->running_threads, pool->threads->idx);
		ticks++;
		usleep(30000);
	}
	return 0;
}

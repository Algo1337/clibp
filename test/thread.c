#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <thread.h>

void test_fn(thread_t t) {
	printf("%d\n", *(int *)t->args[0]);
	for(int i = 0; i < 30; i++) {
		printf("Hi %d\n", i);
		sleep(1);
	}
}

int main() {
	int g = 1;
	thread_t thread = create_n_execute_thread(test_fn, (void *[]){(void *)&g, NULL}, 1);

	thread_wait(thread);
	return 0;
}

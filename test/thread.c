#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <thread.h>

void test_fn() {
	for(int i = 0; i < 30; i++) {
		printf("Hi %d\n", i);
		sleep(1);
	}
}

int main() {
	thread_t thread = create_n_execute_thread(test_fn, NULL, 0);

	thread_wait(thread);
	return 0;
}

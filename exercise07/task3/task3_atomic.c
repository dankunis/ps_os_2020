#include <cross-stdarg.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 1000
#define NUM_LOOPS 10000

struct shared {
	_Atomic int X;
};

void* thread_work(void* X) {
	struct shared* s = (struct shared*)X;
	for(int i = 0; i < NUM_LOOPS; i++) {
		s->X += 1;
	}

	return NULL;
}

int main() {
	struct shared* s = malloc(sizeof(*s));

	// create threads
	pthread_t t[NUM_THREADS];
	for(int i = 0; i < NUM_THREADS; i++) {
		int err = pthread_create(&t[i], NULL, thread_work, (void*)s);

		if(err != 0) {
			perror("Error creating thread number");
		}
	}

	for(int i = 0; i < NUM_THREADS; i++) {
		pthread_join(t[i], NULL);
	}

	printf("Shared value %d \n", s->X);

	free(s);

	return EXIT_SUCCESS;
}

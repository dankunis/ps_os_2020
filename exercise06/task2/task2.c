#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "myqueue.h"

pthread_mutex_t lock;
myqueue q = STAILQ_HEAD_INITIALIZER(q);

void* thread_work() {
	unsigned sum = 0;
	int nextValue = -1;

	while(nextValue != 0) {
		pthread_mutex_lock(&lock);
		if(!myqueue_is_empty(&q)) {
			nextValue = myqueue_pop(&q);
			sum += nextValue;
		}
		pthread_mutex_unlock(&lock);
	}

	printf("Thread id %ld: sum = %u\n", (long int)pthread_self(), sum);
	return (void*)(intptr_t)sum;
}

int main() {
	myqueue_init(&q);

	if(pthread_mutex_init(&lock, NULL) != 0) {
		perror("pthread_mutex_init");
		exit(EXIT_FAILURE);
	}

	pthread_t t[5];
	for(int i = 0; i < 5; i++) {
		if(pthread_create(&t[i], NULL, thread_work, NULL) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	pthread_mutex_lock(&lock);
	for(int i = 0; i < 10005; i++) {
		if(i >= 10000) {
			myqueue_push(&q, 0);
		} else {
			myqueue_push(&q, 1);
		}
	}
	pthread_mutex_unlock(&lock);

	unsigned totalSum = 0;

	for(int i = 0; i < 5; i++) {
		void* threadSum;
		pthread_join(t[i], &threadSum);
		totalSum += (unsigned int)threadSum;
	}

	pthread_mutex_destroy(&lock);

	printf("total sum = %d\n", totalSum);

	return EXIT_SUCCESS;
}

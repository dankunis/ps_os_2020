#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../myqueue.h"

pthread_mutex_t lock;
pthread_cond_t q_cv = PTHREAD_COND_INITIALIZER;
myqueue q = STAILQ_HEAD_INITIALIZER(q);

void* thread_work() {
	unsigned sum = 0;
	int nextValue = -1;

    while(nextValue != 0) {
		pthread_mutex_lock(&lock);

		while(myqueue_is_empty(&q)) {
			pthread_cond_wait(&q_cv, &lock);
		}
		nextValue = myqueue_pop(&q);

		pthread_mutex_unlock(&lock);

		sum += nextValue;
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

	pthread_t t[50];
	for(int i = 0; i < 50; i++) {
		if(pthread_create(&t[i], NULL, thread_work, NULL) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	for(int i = 0; i < 10050; i++) {
		unsigned short newValue = i >= 10000 ? 0 : 1;

		pthread_mutex_lock(&lock);

		myqueue_push(&q, newValue);

		pthread_cond_signal(&q_cv);
		pthread_mutex_unlock(&lock);
	}

	unsigned totalSum = 0;

	for(int i = 0; i < 50; i++) {
		void* threadSum;
		pthread_join(t[i], &threadSum);
		totalSum += (uintptr_t)threadSum;
	}

	pthread_mutex_destroy(&lock);

	printf("total sum = %d\n", totalSum);

	return EXIT_SUCCESS;
}

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../myqueue.h"

pthread_spinlock_t lock;
myqueue q = STAILQ_HEAD_INITIALIZER(q);

void* thread_work() {
	unsigned sum = 0;
	int nextValue = -1;

	while(nextValue != 0) {
		pthread_spin_lock(&lock);
		if(!myqueue_is_empty(&q)) {
			nextValue = myqueue_pop(&q);
			sum += nextValue;
		}
		pthread_spin_unlock(&lock);
	}

	printf("Thread id %ld: sum = %u\n", (long int)pthread_self(), sum);
	return (void*)(intptr_t)sum;
}

int main() {
	myqueue_init(&q);

	if(pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE) != 0) {
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

	pthread_spin_lock(&lock);
	for(int i = 0; i < 10050; i++) {
		if(i >= 10000) {
			myqueue_push(&q, 0);
		} else {
			myqueue_push(&q, 1);
		}
	}
	pthread_spin_unlock(&lock);

	unsigned totalSum = 0;

	for(int i = 0; i < 50; i++) {
		void* threadSum;
		pthread_join(t[i], &threadSum);
		totalSum += (uintptr_t)threadSum;
	}

	pthread_spin_destroy(&lock);

	printf("total sum = %d\n", totalSum);

	return EXIT_SUCCESS;
}

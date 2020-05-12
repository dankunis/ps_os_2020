#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 1000
#define NUM_LOOPS 10000

pthread_mutex_t lock;

void *thread_work(void *shared_int) {
    int *shared = (int*) shared_int;

    for (int i = 0; i < NUM_LOOPS; i++) {
        pthread_mutex_lock(&lock);
        *shared += 1;
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    int *X = malloc(sizeof(*X));
    *X = 0;

    if(pthread_mutex_init(&lock, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }

    // create threads
    pthread_t t[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        int err = pthread_create(&t[i], NULL, thread_work, (void*) X);

        if (err != 0) {
            perror("Error creating thread number");
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t[i], NULL);
    }

    printf("Shared value %d \n", *X);

    free(X);
    pthread_mutex_destroy(&lock);

    return EXIT_SUCCESS;
}

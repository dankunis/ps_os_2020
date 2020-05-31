#include <assert.h>
#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "allocator.h"

size_t N = 0;
size_t S = 0;

unsigned int rand_seed = RANDOM_SEED;

void* thread_fn() {
	void* block[N + N / 2];

	/* perform N allocations of random sizes between S and 8*S */
	for(size_t i = 0; i < N; ++i) {
		size_t alloc_size = S * (1 + rand_r(&rand_seed) % 8);
		block[i] = my_malloc(alloc_size);
		if(block[i] == NULL) {
			fprintf(stderr, "my_alloc returned NULL - out of memory?!\n");
			abort();
		}
	}

	/* free 50% of mem blocks randomly */
	for(size_t i = 0; i < N; ++i) {
		if(block[i] != NULL && rand_r(&rand_seed) % 2) {
			my_free(block[i]);
			block[i] = NULL;
		}
	}

	/* Allocate another N/2 blocks of random size */
	for(size_t i = 0; i < N / 2; ++i) {
		size_t alloc_size = S * (1 + rand_r(&rand_seed) % 8);
		block[i + N] = my_malloc(alloc_size);
		if(block[i] == NULL) {
			fprintf(stderr, "my_alloc returned NULL - out of memory?!\n");
			abort();
		}
	}

	/* free all allocated blocks */
	for(size_t i = 0; i < N + N / 2; ++i) {
		if(block[i] != NULL) {
			my_free(block[i]);
			block[i] = NULL;
		}
	}

	return NULL;
}

int main(int argc, char* argv[]) {
	if(argc != 5) {
		printf("Usage: %s <allocator.so> <nthreads> <nallocs> <allocsize>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* load the library and register functions */
	void* allocator = dlopen(argv[1], RTLD_NOW);
	if(!allocator) {
		fprintf(stderr, "dlopen failure %s\n", dlerror());
		return EXIT_FAILURE;
	};

	my_malloc = dlsym(allocator, "my_malloc");
	if(!my_malloc) {
		fprintf(stderr, "dlsym my_malloc failure %s\n", dlerror());
		exit(EXIT_FAILURE);
	};
	my_free = dlsym(allocator, "my_free");
	if(!my_free) {
		fprintf(stderr, "dlsym my_free failure %s\n", dlerror());
		exit(EXIT_FAILURE);
	};

	size_t num_threads = atol(argv[2]);
	assert(num_threads <= 1000);
	N = atol(argv[3]);
	assert(N <= 1e6);
	S = atol(argv[4]);
	assert(S <= (2 << 20));

	printf("Running %lu threads, each doing %lu allocations of size %lu using %s\n", num_threads, N,
	       S, argv[1]);

	pthread_t* thread_handles = calloc(num_threads, sizeof(pthread_t));
	for(size_t i = 0; i < num_threads; ++i) {
		pthread_create(&thread_handles[i], NULL, thread_fn, NULL);
	}
	for(size_t i = 0; i < num_threads; ++i) {
		pthread_join(thread_handles[i], NULL);
	}
	free(thread_handles);
	dlclose(allocator);

	return EXIT_SUCCESS;
}

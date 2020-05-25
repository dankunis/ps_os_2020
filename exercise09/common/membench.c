#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "allocator.h"

size_t N = 0;
size_t S = 0;

void* thread_fn() {
	for(size_t i = 0; i < N; ++i) {
		void* ptr = my_malloc(S);
		if(ptr == NULL) {
			fprintf(stderr, "my_alloc returned NULL - out of memory?!\n");
			abort();
		}
		my_free(ptr);
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

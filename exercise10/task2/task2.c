#include "free_list_allocator.h"

// List structure for free list
typedef struct list {
	struct header* next;
	struct header* last;
	size_t size;
} list_t;

// Header structure for memory blocks
typedef struct header {
	struct header* next;
	size_t size;
} header_t;

#ifdef LOCAL_ALLOCATOR
__thread list_t* slabs[SLAB_MAX + 1];
__thread void* memblock;
char* alloc_mode = "local";
#else
list_t* slabs[SLAB_MAX + 1];
void* memblock;
pthread_mutex_t mutex;
char* alloc_mode = "global";
#endif

/*
 * Initializes allocator by reserving a big memory block from OS
 * and initializing free list.
 */
int init_allocator() {
	// Allocate memory block
	if((memblock = calloc(1, MEMBLOCK_SIZE)) == NULL) {
		fprintf(stderr, "[Allocator] Can't reserve memory block from OS.\n");
		return -1;
	}

	// Inititilize free list
	slabs[0] = calloc(1, sizeof(list_t));
	slabs[0]->size = MEMBLOCK_SIZE;
	slabs[0]->next = memblock;
	slabs[0]->last = memblock;

	for(int i = 1; i <= SLAB_MAX; i++) {
		slabs[i] = calloc(1, sizeof(list_t));
		slabs[i]->next = NULL;
		slabs[i]->size = 0;
	}

	// Set destroy_allocator() as exit/cleanup routine
#ifdef LOCAL_ALLOCATOR
	pthread_key_t key;
	pthread_key_create(&key, destroy_allocator);
	pthread_setspecific(key, slabs[0]->next);
#else
	atexit(destroy_allocator);
#endif

	printf("[Allocator] Initialization complete. Running in %s mode. block size: %ld\n", alloc_mode,
	       sizeof(header_t));

	return 0;
}

/*
 * Destroys Memory allocator, get's called by atexit(3), no need to do it manually.
 */
void destroy_allocator() {
	if(memblock != NULL) {
		for(int i = 0; i <= SLAB_MAX; i++) {
			free(slabs[i]);
		}
		free(memblock);
		memblock = NULL;
		printf("[Allocator] Memory block freed.\n");
	}
}

void* my_malloc(size_t size) {

#ifndef LOCAL_ALLOCATOR
	pthread_mutex_lock(&mutex);
#endif

	// Initialize Allocator, if not already done
	if(memblock == NULL) {
		if(init_allocator() == -1) {
			return NULL;
		}
	}

	int slab = (int)ceil(log2(size));
	header_t* block = NULL;

	// Check if a free suitable block is available
	if(slabs[slab]->next != NULL) {
		block = slabs[slab]->next;
		slabs[slab]->next = slabs[slab]->next->next;
	}

	// Otherwise split a new block from unallocated space
	if(slabs[0]->size >= size + sizeof(header_t)) {
		void* shifter = slabs[0]->last;
		shifter += sizeof(header_t) + size;
		block = (header_t*)slabs[0]->last;
		block->size = size;
		slabs[0]->size -= (size + sizeof(header_t));
		slabs[0]->last = shifter;
	} else {
#ifndef LOCAL_ALLOCATOR
		pthread_mutex_unlock(&mutex);
#endif
		fprintf(stderr, "[Allocator] Not enough memory left for allocation.\n");
		return NULL;
	}

#ifndef LOCAL_ALLOCATOR
	pthread_mutex_unlock(&mutex);
#endif

	// Increase, to return data segment
	block++;

	return block;
}

void my_free(void* block) {
#ifndef LOCAL_ALLOCATOR
	pthread_mutex_lock(&mutex);
#endif

	// Decrease to set pointer to header
	header_t* delete = block;
	delete --;

	int slab = (int)ceil(log2(delete->size));

	// Add block to slab
	if(slabs[slab]->next == NULL) {
		slabs[slab]->next = delete;
	} else {
		header_t* current = slabs[slab]->next;
		while(current->next != NULL) {
			current = current->next;
		}
		current->next = delete;
	}

#ifndef LOCAL_ALLOCATOR
	pthread_mutex_unlock(&mutex);
#endif
}

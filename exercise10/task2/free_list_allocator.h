#ifndef TASKS_FREE_LIST_ALLOCATOR_H
#define TASKS_FREE_LIST_ALLOCATOR_H

#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Memory size to manage
#define MEMBLOCK_SIZE 1024 * 1024 * 256

// Maximum slab size as power of 2, e.g. 2^8 = 256 bytes
#define SLAB_MAX 20

/* Function headers */

int init_allocator();

void destroy_allocator();

void* my_malloc(size_t size);

void my_free(void* block);

#endif // TASKS_FREE_LIST_ALLOCATOR_H

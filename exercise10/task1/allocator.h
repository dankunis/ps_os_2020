#ifndef TASKS_ALLOCATOR_H
#define TASKS_ALLOCATOR_H

#include "stdlib.h"

#define RANDOM_SEED 1024;

void* (*my_malloc)(size_t);
void (*my_free)(void*);

#endif // TASKS_ALLOCATOR_H

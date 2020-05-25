#ifndef TASKS_ALLOCATOR_H
#define TASKS_ALLOCATOR_H

#include "stdlib.h"

//void* my_malloc(size_t size);
void* (*my_malloc)(size_t);
//void my_free(void* ptr);
void (*my_free)(void*);

#endif // TASKS_ALLOCATOR_H

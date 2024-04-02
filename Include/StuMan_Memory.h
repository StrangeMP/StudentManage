#pragma once
#include <stdlib.h>
#ifdef DEBUG
#include <stddef.h>
#define MALLOC my_alloc
#define FREE my_free
typedef struct {
    int cnt;
    void *table[2048];
} MemoryRecord;
extern MemoryRecord memRec;
void *my_alloc(size_t _Size);
void my_free(void *_Memory);
#else
#define MALLOC malloc
#define FREE free
#endif

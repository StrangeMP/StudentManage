#pragma once
#include <stdlib.h>
#ifdef DEBUG
#include <stddef.h>
#ifdef MALLOC
#undef MALLOC
#endif
#ifdef FREE
#undef FREE
#endif
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
#ifndef MALLOC
#define MALLOC malloc
#endif
#ifndef FREE
#define FREE free
#endif
#endif

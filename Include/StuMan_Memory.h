#pragma once
#include <stddef.h>
typedef struct {
    int cnt;
    void *table[32768];
} MemoryRecord;
extern MemoryRecord memRec;
void *MALLOC(size_t size);
void FREE(void *ptr);
void ReleaseResource();
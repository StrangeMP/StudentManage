#include "StuMan_Memory.h"
#ifdef DEBUG
MemoryRecord memRec = {0, {NULL}};
void *my_alloc(size_t _Size) {
    memRec.table[memRec.cnt] = malloc(_Size);
    return memRec.table[memRec.cnt++];
}
void my_free(void *_Memory) {
    free(_Memory);
    for (int i = 0; i < memRec.cnt; i++) {
        if (memRec.table[i] == _Memory) {
            memRec.table[i] = NULL;
            break;
        }
    }
}
#endif

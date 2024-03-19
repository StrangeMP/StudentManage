#include "StuMan_Student.h"
#include "StuMan_Search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Student_Node *idIndex[90][4][32][32] = {NULL};            // 学号-学生内存地址索引
Student_List *nameIndex[65536] = {NULL};             // 名字-学号索引
Student_List *gradeIndex[90][4] = {{0, NULL, NULL}}; // 学院年级-学号索引

struct STUMAN_DataAddress data_address = {NULL, NULL, NULL, NULL};

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
            --i;
        }
    }
}
#endif

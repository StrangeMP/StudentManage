#include "Student.h"
struct Student *idIndex[90][4][30][30]; // 学号-学生内存地址索引

// id =212305
void buildIDIndex(Student *pStu) {
    int id = pStu->id;
    idIndex[id / 1000000][id / 10000 % 100][id / 100 % 100][id % 100] = pStu;
}

#include "Student.h"
Student *idIndex[90][4][30][30]; // 学号-学生内存地址索引

// id =212305
void buildIDIndex(Student *pStu) {
    int id = pStu->id;
    idIndex[id / 1000000][id / 10000 % 100][id / 100 % 100][id % 100] = pStu;
}

size_t Get16bitHash(const wchar_t str[]) {
    int hash = 0;
    int len = 0;
    for (; str[len] != L'\0'; len++)
        ;
    const wchar_t *ch = str;
    for (int i = 0; i < len; i++) {
        hash = hash + ((hash) << 5) + *(ch + i) + ((*(ch + i)) << 7);
    }
    return ((hash) ^ (hash >> 16)) & 0xffff;
}

int nameIndex[65536];
void buildNameIndex(Student *pStu) { nameIndex[Get16bitHash(pStu->name)] = pStu->id; }
Student *findStu_by_ID(const int id) {
    return idIndex[id / 1000000][id / 10000 % 100][id / 100 % 100][id % 100];
}
Student *findStu_by_name(const wchar_t name[]) {
    int id = nameIndex[Get16bitHash(name)];
    return findStu_by_ID(id);
}
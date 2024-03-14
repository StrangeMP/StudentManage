#include "StuMan_Search.h"
#include <string.h>

size_t Get_16bit_Hash(const char str[]) {
    int hash = 0;
    int len = 0;
    for (; str[len] != L'\0'; len++)
        ;
    const char *ch = str;
    for (int i = 0; i < len; i++) {
        hash = hash + ((hash) << 5) + *(ch + i) + ((*(ch + i)) << 7);
    }
    return ((hash) ^ (hash >> 16)) & 0xffff;
}

Student *Get_Student_by_id(const int id) {
    return idIndex[id / 1000000][id / 10000 % 100][id / 100 % 100][id % 100];
}

const Student_List *Get_StudentList_by_name(const char name[]) {
    Student_List *id_List = nameIndex[Get_16bit_Hash(name)];
    return id_List;
}

const Student_List *Get_StudentList_by_grade(int institute_and_grade) {
    return gradeIndex[institute_and_grade / 100][institute_and_grade % 100];
}

const Student_List *Get_StudentList_by_CourseID(const char *course_id) {
    return Get_Course(course_id)->followed;
}

Course *Get_Course(const char *course_id) {
    Course_Node *crs_node = data_address.pCourseHead;
    if (crs_node == NULL)
        return NULL;
    while (true) {
        if (strcmp(crs_node->crs.id, course_id) == 0)
            return &crs_node->crs;
        else if (crs_node->next == NULL)
            break;
        else
            crs_node = crs_node->next;
    }
    // not found
    return NULL;
}

Enroll *Enroll_Find(Student *src, const char *course_id) {
    if (src->enrolled == NULL)
        return NULL;
    Enroll *enroll_node = src->enrolled;
    while (true) {
        if (strcmp(enroll_node->course_id, course_id) == 0)
            return enroll_node;
        else if (enroll_node->next == NULL)
            return NULL;
        else
            enroll_node = enroll_node->next;
    }
}
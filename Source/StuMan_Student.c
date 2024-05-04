#include "StuMan_Student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Student_Node *idIndex[90][4][32][32] = {NULL};       // 学号-学生内存地址索引
Student_List *nameIndex[65536] = {NULL};             // 名字-学号索引
Student_List *gradeIndex[90][4] = {{0, NULL, NULL}}; // 学院年级-学号索引

struct STUMAN_DataAddress data_address = {.student_total = 0,
                                          .course_total = 0,
                                          .pCourseFoot = NULL,
                                          .pCourseHead = NULL,
                                          .pStudentHead = NULL,
                                          .pStudentFoot = NULL};

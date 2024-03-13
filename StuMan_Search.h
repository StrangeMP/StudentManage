#pragma once
#include "StuMan_Student.h"
#include <string.h>
Student *Get_Student_by_id(const int id);
Student_IdNode *Get_Student_IdNode_by_id(const int id);
const Student_List *Get_StudentList_by_name(const char name[]);
const Student_List *Get_StudentList_by_grade(int institute_and_grade);
const Student_List *Get_StudentList_by_CourseID(const char *course_id);
Course *Get_Course(const char *course_id);
size_t Get_16bit_Hash(const char str[]);
Enroll *Enroll_Find(Student *src, const char *course_id);
#pragma once
#include "StuMan_Student.h"
#include <string.h>
// Returns the Student from database
Student *Get_Student_by_id(const int id);

// Returns a Student_Node* from database
// This function should only be used by deleter.
Student_Node *Get_Student_Node_by_id(const int id);
Student_List *Get_StudentList_by_name(const char name[]);
Student_List *Get_StudentList_by_grade(int institute_and_grade);
Student_List *Get_StudentList_by_CourseID(const char *course_id);
Course *Get_Course(const char *course_id);
size_t Get_16bit_Hash(const char str[]);
Enroll *Enroll_Find(Student *src, const char *course_id);
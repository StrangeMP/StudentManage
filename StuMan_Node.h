#pragma once
#include "StuMan_Student.h"
Student_IdNode *Student_IdNode_Find(Student_IdNode *Head, int id);
Student_IdNode *Student_IdNode_Add(Student_IdNode *Head, int id);
Student_List *Student_List_AddStudentID(Student_List *stu_list, int id);
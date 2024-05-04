#pragma once
#include "VECTOR.h"
VECTOR(StrVec, char *)

struct Teacher {
    char name[32];
    char id[20];
    char PW_MD5[33];
    StrVec *courses;
};

const char *Get_Student_PWMD5(int stu_id);
struct Teacher *Get_Teacher(const char *teacher_id);
void Teacher_Signup(const char *fileDir);
void CleanTeachers();
#pragma once
#define STUDENT 1
#define COURSE 2
#define STU_IN_COURSE 3
#define BENIFIT 4

//STUDENT,1,int id:del stu
//COURSE,1,course_id:del course
//STU_IN_COURSE,1,char *course_id:del all stu in course
//
void del(int which, int num, ...);
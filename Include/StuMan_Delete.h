#pragma once
#include"StuMan_Student.h"
#define STU 1
#define CRS  2
#define STU_IN_CRS 3
#define CRS_OF_STU 4

//CRS , 1, course_id: del course
//STU , 1, int    id: del student
//STU_IN_CRS, 1, char * course_id   :del all stu in course
//CRS_OF_STU, 2, int id,char *course:del course of student
void del(int which, int num, ...);

//增加需要获奖pending的学生,
void AddpdList(int id);
//更新获奖学生数据
void Update_pd(int id);
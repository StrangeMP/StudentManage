#pragma once
#define STU 1
#define CRS 2
#define CRS_OF_STU 4

// CRS , 1, course_id: del course
// STU , 1, int    id: del student
// CRS_OF_STU, 2, int id,char *course:del course of student
void del(int which, int num, ...);

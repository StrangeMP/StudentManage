#pragma once
#include "StuMan_Student.h"
#include <stddef.h>
// 更新学生的不及格课程数、学业成绩、学科素质加分分数
void Student_Update(int stu_id);

// 更新某学院某专业某年级学生的排名
// e.g. UpdateRank(2123,2100); updates ranking for 2023级 计算机科学与技术
void UpdateRank(const int institute_grade, const int professionNum);

// Collect an array of Student* and place it at _dest
size_t Extract_Students(Student ***_dest, const int institute_grade, const int professionNum);

// 更新课程的优秀率、合格率
void Course_Update(const char *course_id);
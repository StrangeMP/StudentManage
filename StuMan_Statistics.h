#pragma once
// 更新学生的不及格课程数、学业成绩、学科素质加分分数
void Student_Update(int stu_id);

// 更新某学院某专业某年级学生的排名
void UpdateRank(int college_num, const char *professionName);

// 更新课程的优秀率、合格率
void Course_Update(const char *course_id);
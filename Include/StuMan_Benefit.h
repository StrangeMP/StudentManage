#pragma once
#include "StuMan_Student.h"

void Student_AddEssay(int stu_id, Essay *essay);
void Student_AddProject(int stu_id, Project *project);
void Student_AddAward(int stu_id, Award *award);
void PendingList_Update(Student *stu);
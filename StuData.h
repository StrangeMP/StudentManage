#pragma once
#include "Student.h"

// Global pointers to loaded data
// Globally used.
struct {
    StuNode *pStuHead;
    StuNode *pStuFoot;
    Course *pCourseHead;
    Course *pCourseFoot;
} LoadResult = {NULL, NULL, NULL, NULL};

// Load data from a binary file.
// Data to load and build includes:
//      1) Student, Course and Awards
//      2) nameIndex and idIndex
void LoadData(FILE *bin);

void ImportData(FILE *json);
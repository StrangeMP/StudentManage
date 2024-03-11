#include "Instit.h"
#include "Student.h"
#include <stdio.h>

enum dataType {
    STU = 1, // Student
    CRS = 2  // Course
};

// The following two functions are used only to LOAD data from a binary file
// They should be wrapped in a load_Data function
// Other setup functions are pending to be added below.
static void Setup_StuLinkedList(FILE *p, Student *pHead);
static void Setup_CoursesLinkedList(FILE *p, Course *pHead);

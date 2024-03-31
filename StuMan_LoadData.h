#pragma once
#include"StuMan_Student.h"
#include <stdio.h>



// The following two functions are used only to LOAD data from a binary file
// They should be wrapped in a load_Data function
// Other setup functions are pending to be added below.
void LinkedList_Binaried();
void LoadData(const char *fileDir1,const char *fileDir2);
void Stu_Write(Student pStu,FILE *f);
void Cor_Write(Course pCor,FILE *f);
bool examin(FILE *p,char a);
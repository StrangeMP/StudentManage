#include"Instit.h"
#include"Student.h"
#include<stdio.h>

void Setup_StuLinkedList(FILE *p,Student *pHead);

void Setup_CoursesLinkedList(FILE *p,Course *pHead);


int main()
{
    char Address[200];                  //导入文件地址
    FILE *pf;
    StuNode *pHead_Stu;
    Course *pHead_Cors;

    printf("Please input the address of the file,including the scores of students:");
    gets(Address);
    while( pf=fopen(Address,"rt") != NULL )
    {
        printf("The file doesn't exit, please retype the address:");
        gets(Address);
    }                                   //输入了错误文件地址

    Setup_StuLinkedList(pf,pHead_Stu);
    Setup_CoursesLinkedList(pf,pHead_Cors);

    fclose(pf);
    printf("Successfully import!");
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include"StuMan_Student.h"
#include"StuMan_Benefit.h"
void *Student_AddEssay(int stu_id, Essay *essays)
{
    // 先找到对应学生节点
    Student *student = Get_Student_by_id(stu_id);

    if (student != NULL)
    {
        // 分配内存给加分论文
        student->Benefits.essays= (Essay *)malloc(sizeof(essays));
        if (student->Benefits.essays == NULL)
        {
            // 内存分配失败
            printf("Memory allocation failed for essay.\n");
            return NULL;
        }

        // 将加分论文添加到学生的加分结构中
        if (student->Benefits.essays == NULL)
        {
            essays->next = NULL;
            essays->prev = NULL;
        }
        else
        {
            essays->next = student->Benefits.essays;
            essays->prev = NULL; // 假设这篇论文将成为新的头部
            student->Benefits.essays->prev = essays;
            student->Benefits.essays = essays;
        }

        // 返回成功添加的加分论文指针
        return essays;
    }
}

void *Student_AddProject(int stu_id, Project *project){
    // 先找到对应学生节点
    Student *student = Get_Student_by_id(stu_id);

    if (student != NULL)
    {
        // 分配内存给加分论文
        student->Benefits.projects= (Project *)malloc(sizeof(project));
        if (student->Benefits.projects== NULL)
        {
            // 内存分配失败
            printf("Memory allocation failed for essay.\n");
            return NULL;
        }

        // 将加分论文添加到学生的加分结构中
        if (student->Benefits.projects == NULL)
        {
            project->next = NULL;
            project->prev = NULL;
        }
        else
        {
            project->next= student->Benefits.essays;
            project->prev = NULL; // 假设这篇论文将成为新的头部
            student->Benefits.projects->prev = project;
            student->Benefits.projects = project;
        }

        // 返回成功添加的加分论文指针
        return project;
    }
}


void *Student_AddAward(int stu_id, Award *award){
        // 先找到对应学生节点
    Student *student = Get_Student_by_id(stu_id);

    if (student != NULL)
    {

        student->Benefits.awards= (Award *)malloc(sizeof(award));
        if (student->Benefits.awards == NULL)
        {
        
            printf("Memory allocation failed for essay.\n");
            return NULL;
        }

        if (student->Benefits.awards == NULL)
        {
            award->next = NULL;
            award->prev = NULL;
        }
        else
        {
            award->next = student->Benefits.awards;
            award->prev = NULL; 
            student->Benefits.awards->prev = award;
            student->Benefits.awards = award;
        }

        // 返回成功添加的加分论文指针
        return award;
    }
}
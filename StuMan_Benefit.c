#include<stdio.h>
#include<stdlib.h>
#include"StuMan_Student.h"
#include"StuMan_Benefit.h"
#include"StuMan_Search.h"
void *Student_AddEssay(int stu_id, Essay *essays)
{   
    Student* student =Get_Student_by_id(stu_id);
    student=(Student*)malloc(sizeof(student));
    if (student != NULL)
    {
        student->Benefits.essays= (Essay *)malloc(sizeof(essays));
        if (student->Benefits.essays == NULL)
        {
            printf("Memory allocation failed for essay.\n");
            return NULL;
        }
        if (student->Benefits.essays == NULL)
        {
            essays->next = NULL;
            essays->prev = NULL;
        }
        else
        {
            essays->next = student->Benefits.essays;
            essays->prev = NULL; 
            student->Benefits.essays->prev = essays;
            student->Benefits.essays = essays;
        }
        return essays;
    }
}

void *Student_AddProject(int stu_id, Project *project){
    Student *student = Get_Student_by_id(stu_id);
    student=(Student*)malloc(sizeof(student));
    if (student != NULL)
    {
        student->Benefits.projects= (Project *)malloc(sizeof(project));
        if (student->Benefits.projects== NULL)
        {
            printf("Memory allocation failed for essay.\n");
            return NULL;
        }

        if (student->Benefits.projects == NULL)
        {
            project->next = NULL;
            project->prev = NULL;
        }
        else
        {
            project->next= student->Benefits.projects;
            project->prev = NULL; 
            student->Benefits.projects->prev = project;
            student->Benefits.projects = project;
        }

        return project;
    }
}


void *Student_AddAward(int stu_id, Award *award){
    Student *student = Get_Student_by_id(stu_id);
    student=(Student*)malloc(sizeof(student));
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

        return award;
    }
}
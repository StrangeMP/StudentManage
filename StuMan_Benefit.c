#include <stdio.h>
#include<stdlib.h>
#include"StuMan_Benefit.h"
#include"StuMan_Student.h"
#include"StuMan_Search.h"
#include"StuMan_Node.h"
#include"StuMan_Import.h"

Student_List Benefits_PendingVerified={0,NULL,NULL};

static Student_List* AddpdList(int id,Student_List* p)
{
    p->student_count++;
    if(p->first==NULL)
    {
        Student_IdNode* q = malloc(sizeof(Student_IdNode));
        q->id = id;
        q->next = NULL;
        q->prev = NULL;
        p->first = q;
        p->end = q;
    }
    else
    {
        Student_IdNode* q = malloc(sizeof(Student_IdNode));
        q->id = id;
        q->next = NULL;
        q->prev = p->end;
        p->end->next = q;
        p->end = q;
    }
    return p;
}

static void *AddEssay(int stu_id, Essay *essay){
    Student* student = Get_Student_by_id(stu_id);
    if(student->Benefits.essays == NULL){
        essay->prev = NULL;
        essay->next = NULL;
        student->Benefits.essays = essay;
        student->Benefits.essays->prev = NULL;
        student->Benefits.essays->next = NULL;
    }else{
        Essay* p = student->Benefits.essays;
        while(p->next != NULL){
            p = p->next;
        }
        p->next = essay;
        essay->prev = p;
        essay->next = NULL;
    
    }
    return NULL;
}


static void *AddProject(int stu_id, Project *project){

    Student*student= Get_Student_by_id( stu_id); // Add the missing function declaration

    if(student->Benefits.projects == NULL){

        project->prev = NULL;
        project->next = NULL;
        student->Benefits.projects = project;
        student->Benefits.projects->prev = NULL;
        student->Benefits.projects->next = NULL;

    }else{

        Project* p = student->Benefits.projects;

        while(p->next != NULL){
            p = p->next;
        }

        p->next = project;
        project->prev = p;
        project->next = NULL;

    }

    return NULL;
}

static void *AddAward(const int stu_id, Award *award){  
    Student* student = Get_Student_by_id(stu_id);
    
   if(student->Benefits.awards == NULL){

        award->prev = NULL;
        award->next = NULL;
        student->Benefits.awards = award;
        student->Benefits.awards->prev = NULL;
        student->Benefits.awards->next = NULL;

   }else {

        Award* p = student->Benefits.awards;
        while (p->next != NULL) 
        {
            p = p->next;
        }
        p->next = award;
        award->prev = p;
        award->next = NULL;

    }

    return NULL ;
}
void*Student_AddEssay(int stu_id,Essay*essay)
{
   AddEssay(stu_id,essay);
   //AddpdList(stu_id,p);
}
void*Student_AddProject(int stu_id,Project*project)
{
    AddProject(stu_id,project);
    //AddpdList(stu_id,p);
}
void*Student_AddAward(int stu_id,Award*award)
{
    AddAward(stu_id,award);
    //AddpdList(stu_id,p);
}




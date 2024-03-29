#include <stdio.h>
#include<stdlib.h>
#include"StuMan_Benefit.h"
#include"StuMan_Student.h"
#include"StuMan_Search.h"
#include"StuMan_Node.h"
#include"StuMan_Import.h"

static Award_List* InitAwardList()
{
    Award_List* p = (Award_List*)malloc(sizeof(Award_List));
    p->first = NULL;
    p->end = NULL;
    p->student_count = 0;
    return p;
}

static Essay_List* IniEssayList()
{
    Essay_List* p = (Essay_List*)malloc(sizeof(Essay_List));
    p->first = NULL;
    p->end = NULL;
    p->student_count = 0;
    return p;
}

static Project_List* InitProjectList()
{
    Project_List *p=(Project_List*)malloc(sizeof(Project_List));
    p->first=NULL;
    p->end=NULL;    
    p->student_count=0;
    return 0;
}



static Award_List* AddAwardList(int id,Award_List* p)
{
    if(p==NULL) {
        InitAwardList();
    }
    else
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
    }
    return p;
}

static Essay_List* AddEssayList(int id,Essay_List*p){
    if(p==NULL){
        IniEssayList();
    }else{
        p->student_count=p->student_count+1;
        if(p->first==NULL){
            Student_IdNode* q=malloc(sizeof(Student_IdNode));
            q->id=id;
            q->next=NULL;
            q->prev=NULL;
            p->first=q;
            p->end=q;
        }else
        {
            Student_IdNode* q=(Student_IdNode*)(sizeof(Student_IdNode));
            q->id=id;
            q->next=NULL;
            q->prev=p->end;
            p->end->next=q;
            p->end=q;
        }
    }
}

static Project_List* AddProjectList(int id,Project_List*p){
    if(p==NULL){
        InitProjectList();
    }else{
        p->student_count=p->student_count+1;
        if(p->first==NULL){
            Student_IdNode* q=malloc(sizeof(Student_IdNode));
            q->id=id;
            q->next=NULL;
            q->prev=NULL;
            p->first=q;
            p->end=q;
        }else{
            Student_IdNode* q=malloc(sizeof(Student_IdNode));
            q->id=id;
            q->next=NULL;
            q->prev=p->end;
            p->end->next=q;
            p->end=q;
        }
    }
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
void*Student_AddEssay(int stu_id,Essay*essay){
   AddEssay(stu_id,essay);
   Essay_List*p;
   int i=0;         //i的作用是判断是否是第一次调用函数，如果是第一次调用函数，就初始化链表，在实际使用中i可以for循环中使用的i，这里只为了测试
   if(i==0){
       p=IniEssayList();
       i++;     //i++是为了下次调用函数时不再初始化链表，要是在循环中改行可删
   }
    p=AddEssayList(stu_id,p);
}
void*Student_AddProject(int stu_id,Project*project){
    AddProject(stu_id,project);
    Project_List*p;
    int i=0;
    if(i==0){
        p=InitProjectList();
        i++;
    }
    p=AddProjectList(stu_id,p);
}
void*Student_AddAward(int stu_id,Award*award){
    AddAward(stu_id,award);
    Award_List*p;
    int i=0;
    if(i==0){
        p=InitAwardList();
        i++;
    }
    p=AddAwardList(stu_id,p);
}




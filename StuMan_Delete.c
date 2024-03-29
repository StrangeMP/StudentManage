#include <stdarg.h>
//#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include"StuMan_Delete.h"
#include"StuMan_Student.h"
#include"StuMan_Search.h"

#define CLEAR_BEN(p) \
{\
    if(p!=NULL)\
    {while(p->next!=NULL){p=p->next;FREE(p->prev);}FREE(p);}\
}

//1,2,Student_List*,id :删除List中的指定id
//2,2,Enroll **,id:删除学生的Enroll
//3,1,Student_List*:删除course的整个Student_List
//4,1,Student*:clear指定学生的benifit
int del_(int which,int num,...)
{   
    va_list valist;
    va_start(valist,num);
    switch(which)
    {   
        //删除List中的指定id
        case 1:
        {   
            Student_List   *pl=va_arg(valist,Student_List *);
            Student_IdNode *p=pl->first;
            int aim_id=va_arg(valist,int);
            while(p!=NULL)
            {   
                if(p->id==aim_id)
                {   
                    if(p->prev!=NULL) 
                    {   
                        p->prev->next=p->next;
                        if(p->next!=NULL) p->next->prev=p->prev;
                        FREE(p);
                    }
                    else
                    {   
                        pl->first=p->next;
                        if(p->next!=NULL) p->next->prev=NULL;
                        FREE(p);
                    }
                    va_end(valist);
                    return 0;
                }
                p=p->next;
            }
            va_end(valist);
            return 1;//未找到该学号的学生
            break;
        }

        //删除学生的Enroll
        case 2:
        {   
            Enroll** p2=va_arg(valist,Enroll **);
            int id=va_arg(valist,int);
            if(*p2==NULL){ va_end(valist);return 1;}//当前该学生没有选课
            else
            while(*p2) 
            {   
                Student_List * p3=Get_StudentList_by_CourseID((*p2)->course_id);
                int flag1=0;
                if(p3==NULL) printf("课程%s没有学生选择\n",(*p2)->course_id);
                //删除课程中的学生
                else flag1=del_(1,2,p3,id);

                if(flag1==1) printf("在课程%s中未找到学号为%d的学生\n",(*p2)->course_id,id);
                Enroll* p4=*p2;
                *p2=(*p2)->next;
                FREE(p4);
            }
            *p2=NULL; 
            break;
        }

        //删除course的整个Student_List
        case 3:
        {   
            Student_List   *pl=va_arg(valist,Student_List *);
            Student_IdNode *p=pl->first;
            if(p==NULL)     { va_end(valist);return 1;}
            while(p->next!=NULL) {p=p->next;FREE(p->prev);}
            FREE(p);
            pl->first=NULL;
            break;
        }
        
        /*
        Essay *essays;
        Project *projects;
        Award *awards;
        } Benefits;
        } Student;
        */
        //clear学生的benifit
        case 4:
        {
            Student *p=va_arg(valist,Student *);
            CLEAR_BEN(p->Benefits.awards);
            p->Benefits.awards=NULL;
            CLEAR_BEN(p->Benefits.projects);
            p->Benefits.projects=NULL;
            CLEAR_BEN(p->Benefits.essays);
            p->Benefits.essays=NULL;
            break;
        }
    }
    va_end(valist);
    return 0;
}

//删除学生的enroll和enroll中的学生
int del_enroll(Enroll** enrolledp,int id)
{   
    int flag=del_(2,2,enrolledp,id);
    return flag;
}

int clear_benefit(Student *p)
{   
    del_(4,1,p);
    return 0;
}
/*  ... ...
    int id;
} Student_IdNode;
    ... ...
    int student_count;
    Student_IdNode *first;
} Student_List;
*/
int del_gradeIndex(int institute_grade,int aim_id)
{   
    int flag1=del_(1,2,(Get_StudentList_by_grade(institute_grade)),aim_id);
    return flag1;
/*
    time_t timep;
    struct tm *p;
    time (&timep);//获取从1900年1月1日0时0分0秒到现在的秒数
    p=gmtime(&timep)
    1900+p->tm_year;
    1+p->tm_mon; mon为0-11所以+1
*/
}

int del_name_index(char* name,int id)
{   
    Student_List *p=Get_StudentList_by_name(name);
    int f=del_(1,2,p,id);
    return f;
}


//删除一个学生
/*  ... ...
    int institute_grade; // e.g. 2123
    Enroll *enrolled;
} Student;
    ... ...
    Student stu;
} Student_Node;
*/
typedef struct 
{   
    int flagenroll;
    int flagname;
    int flaggrade;
    int flag;
} flag;

flag del_Stu(int id)
{   
    Student *p=Get_Student_by_id(id); 
    flag f={0,0,0,0};
    if(p==NULL){f.flag=1;return f;}
    //由enroll删除课程中的学生
    //并删除学生的enroll
    f.flagenroll=del_enroll(&(p->enrolled),id);
    //删除学生的benefit
    clear_benefit(p);
    //删除nameindex里面的学生
    f.flagname=del_name_index(p->name,id);
    //删除gradeindex学生
    f.flaggrade=del_gradeIndex(p->institute_grade,id);//time.h暂停使用
    //删除学生本身
    Student_Node *stu=Get_Student_Node_by_id(id);
    if(stu->prev!=NULL)
    {   
        if(stu->next!=NULL) stu->next->prev=stu->prev;
        stu->prev->next=stu->next;
    }
    else 
    {   
        data_address.pStudentHead=stu->next;
        if(stu->next!=NULL)stu->next->prev=NULL;
    }
    FREE(stu);
    return f;
}

//删除一个benifit
int del_Abenift()
{  
    return 0; 
}

/*  ... ...
    char id[13];            // 课程号 e.g. ae22931102
} Course;
    ... ...
    Course crs;
} Course_Node;
*/
int  del_stu_in_course(char *course_id)
{   
    Course *p=Get_Course(course_id);
    if(p==NULL){ printf("未找到课程号为%s的课程\n",course_id); return 1;}
    //删除选课的学生
    int f=del_(3,2,p->followed);
    if(f==1)   {printf("该课程没有学生选课\n");                return 11;}
    return 0;
}

int del_course(char *course_id)
{   //删除选课的学生
    int f=0;
    f=del_stu_in_course(course_id);
    Course_Node* cp=Get_Course_Node(course_id);
    //删除课程
    if(cp->prev!=NULL)
    {   
        if(cp->next!=NULL) cp->next->prev=cp->prev;
        cp->prev->next=cp->next;
    }
    else 
    {   
        data_address.pCourseHead=cp->next;
        if(cp->next!=NULL)cp->next->prev=NULL;
    }
    return f;  
}



//1,1,id:del stu
//2,1,course_id:del course
//3,1,course_id:del all stu in course
void del(int which, int num, ...)
{   va_list valist;
    va_start(valist,num);
    switch(which)
    {

    //删除学生
    case 1:
    {
    int id=va_arg(valist,int);
    int err=0;
    flag flag=del_Stu(id);
    if(flag.flag==1) {printf("未找到该学生\n");return;}
    else
    {
        if(flag.flagenroll==1) {printf("当前该学生没有选课\n");err=1;}
        if(flag.flaggrade==1 ) {printf("在年级索引中未找到学号为%d的学生\n",id);err=1;}
        if(flag.flagname==1  ) {printf("在姓名索引中未找到学号为%d的学生\n",id);err=1;}
    }

    if(err==0)printf("已删除学号为%d的学生\n",id);
    break;
    }

    //删除课程
    case 2:
    {
    char *course_id=va_arg(valist,char *);
    int f=del_course(course_id);
    if(f==1)       printf("未找到课程号为%s的课程\n",course_id);
    else if(f==11) printf("该课程没有学生选课\n");
    else if(f==0)  printf("已删除课程号为%s的课程\n",course_id);
    break;
    }

    //删除课程中的所有学生
    case 3:
    {
    char *course_id=va_arg(valist,char *);
    int f=del_stu_in_course(course_id);
    if(f==0)printf("已清空课程号为%s的课程中的学生\n",course_id);
    else printf("清空失败\n");
    break;
    }

    //删除学生的某个benefit
    case 4:
    {
        break;
    }
    }
    va_end(valist);
    return;
}
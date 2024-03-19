#include <stdarg.h>
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include"test_delete.h"
#include"StuMan_Student.h"
#include"StuMan_Search.h"

//1,2,Student_IdNode,id :删除List中的指定id
//2,2,Enroll,id:删除学生的Enroll
//3,1,Student_List:删除course的整个Student_List
void del_(int which,int num,...)
{
    va_list valist;
    va_start(valist,num);
    switch(which)
    {
        case 1:
        {
            Student_IdNode *p=va_arg(valist,Student_IdNode *);
            int aim_id=va_arg(valist,int);
            while(p!=NULL)
            {
                if(p->id==aim_id)
                {
                    if(p->prev!=NULL) 
                    {
                        p->prev->next=p->next;
                        p->next->prev=p->prev;
                        FREE(p);
                    }
                    else
                    {
                        p->next->prev=NULL;
                        FREE(p);
                    }
                    va_end(valist);
                    return ;
                }
                p=p->next;
            }
            printf("未找到学号为%d的学生",aim_id);
            va_end(valist);
            return;
            break;
        }

        case 2:
        {
            Enroll* p2=va_arg(valist,Enroll *);
            int id=va_arg(valist,int);
            if(p2==NULL){printf("当前该学生没有选课"); va_end(valist);return;}
            while(p2->next!=NULL) 
            {
                del_(1,2,(Get_StudentList_by_CourseID(p2->course_id))->first,id);
                p2=p2->next;
                FREE(p2->prev);
            }
            del_(1,2,(Get_StudentList_by_CourseID(p2->course_id))->first,id);
            FREE(p2);
            break;
        }
        case 3:
        {
            Student_List *pl=va_arg(valist,Student_List *);
            if(pl==NULL){printf("当前该课程没有学生选课"); va_end(valist);return;}
            Student_IdNode *p3=pl->first;
            while(p3->next!=NULL) 
            {
                p3=p3->next;
                FREE(p3->prev);
            }
            FREE(p3);
            FREE(pl);
            break;
        }
    }
    va_end(valist);
    return;
}

//1.1删除学生的enroll和enroll中的学生
void del_enroll(Enroll* enrolled,int id)
{
    //由enroll删除课程中的学生
    del_(2,2,enrolled,id);
    return;
}

//3删除nameindex里面的学生
//4删除gradeindex学生
/*
typedef struct Student_IdNode {
    int id;
    struct Student_IdNode *next;
} Student_IdNode;

typedef struct {
    int student_count;
    Student_IdNode *first;
    Student_IdNode *end;
} Student_List;
*/

/*
Student_Node *idIndex[90][4][32][32] = {NULL};            // 学号-学生内存地址索引
Student_List *nameIndex[65536] = {NULL};             // 名字-学号索引
Student_List *gradeIndex[90][4] = {{0, NULL, NULL}}; // 学院年级-学号索引
*/
void del_gradeIndex(int institute_grade,int aim_id)
{
    del_(1,2,(Get_StudentList_by_grade(institute_grade))->first,aim_id);
    return;
/*
    time_t timep;
    struct tm *p;
    time (&timep);//获取从1900年1月1日0时0分0秒到现在的秒数
    p=gmtime(&timep)
    1900+p->tm_year;
    1+p->tm_mon; mon为0-11所以+1
*/
}

void del_name_index(char* name,int id)
{
    del_(1,2,nameIndex[Get_16bit_Hash(name)]->first,id);
    return;
}



//删除一个学生
//Student *Get_Student_by_id(const int id);
/*
typedef struct {
    char name[32];
    int id;              // e.g. 20230501
    int institute_grade; // e.g. 2123
    Enroll *enrolled;
} Student;

typedef struct Student_Node {
    Student stu;
    struct Student_Node *next;
} Student_Node;

typedef struct Enroll {
    char course_id[13]; // 课程号 e.g. ae22931102
    union {
        double grade; // 百分制 总成绩
        char level;   // 五级制成绩 1优秀/ 2良好/ 3中等/ 4及格/ 5不及格
    };
    char retake;    // 1初修/ 2重修/ 3重考
    int semester;   // 学年学期 231 for 2023-2024学年第1学期
    bool major;     // 是否主修
    double gpa;     // 绩点
    bool passed;    // 是否及格
    bool effective; // 是否有效

    struct Enroll *prev;
    struct Enroll *next;
} Enroll;
*/

void del_Stu(int id)
{
    Student *p=Get_Student_by_id(id);
    //由enroll删除课程中的学生
    //删除学生的enroll
    del_enroll(p->enrolled,id);
    //删除nameindex里面的学生
    del_name_index(p->name,id);
    //删除gradeindex学生
    del_gradeIndex(p->institute_grade,id);//??
    FREE(p);
    return;
}

/*
typedef struct Course {
    char name[50];          // 课程名 e.g.大学物理B
    char id[13];            // 课程号 e.g. ae22931102
    char num;               // 同一课程不同老师用此区分
    char teacher[32];       // 任课教师
    int pub_category;       // 校公选课类别
    int type;               // 课程类别 e.g. 通识基础课
    char nature;            // 课程性质 1必修/ 2选修/ 3限选
    double credit;          // 学分
    int hours;              // 学时
    char exam_date[11];     // 考试日期 e.g. "2023-06-26"
    char grade_type;        // 1百分制/ 2五级制
    char exam_type;         // 1考试 /2考察
    int institute;          // 开课单位
    Student_List *followed; // 选这门课的学生的学号的链表

    // struct Course *prev;
    // struct Course *next;
} Course;

typedef struct Course_Node {
    Course crs;
    struct Course_Node *prev;
    struct Course_Node *next;
} Course_Node;
*/
void del_course(char *course_id)
{
    Course *p=Get_Course(course_id);
    if(p==NULL)
    {
        printf("未找到课程号为%s的课程",course_id);
        return;
    }
    //删除选课的学生
    del_(3,2,p->followed);
    //删课程
    FREE(p);
    return;
}



//1,1,id:del stu
//2,1,course_id:del course
void del(int which, int num, ...)
{
    va_list valist;
    va_start(valist,num);
    switch(which)
    {
    case 1:
    del_Stu(va_arg(valist,int));
    break;
    case 2:
    del_course(va_arg(valist,char*));
    break;
    }
    va_end(valist);
}
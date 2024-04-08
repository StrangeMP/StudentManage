#pragma once
#include <stdbool.h>
#ifndef NULL
#define NULL 0LL
#endif

typedef struct Student_IdNode {
    int id;
    struct Student_IdNode *prev;
    struct Student_IdNode *next;
} Student_IdNode;

typedef struct {
    int student_count;
    Student_IdNode *first;
    Student_IdNode *end;
} Student_List;

// 课程信息
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
    double ExcellentRate;   // 课程优秀率
    double PassRate;        // 课程及格率
    Student_List *followed; // 选这门课的学生的学号的链表

    // struct Course *prev;
    // struct Course *next;
} Course;

typedef struct Course_Node {
    Course crs;
    struct Course_Node *prev;
    struct Course_Node *next;
} Course_Node;

// 学生选课信息
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

extern Student_List * Benefits_PendingVerified;

typedef enum { // 设置枚举变量显示状态
    PENDING,   // 受理
    APPROVED,  // 通过
    DENIED     // 否决
} Status;

typedef struct Essay // 设置Essay结构体变量
{
    char Author[5][32]; // 论文作者 例如:Zhang Wei,Li Hua etc
    char Title[150]; // 论文名   例如:"互联网+"背景下快时尚服装品牌网络营销研究
    char Journal_Conference_Name[150]; // 期刊或会议名
    char PubDate[11];                  // 发表日期   例2024-03-21
    char VIAP[100]; // /*Volume卷/Issue期/Article Number论文号/Paper Range页码范围*/可以没有填NULL
    char Classification[20]; // 论文等级
    double AddGPA;           // 加分
    struct Essay *next;
    struct Essay *prev;
    Status status; // 提交状态
} Essay;

typedef struct Project // 设置Projec结构体变量
{
    char Member[5][32];    // 项目成员
    char GuideTeacher[32]; // 指导教师
    char ProjectID[20];    // 项目编号
    char ProjectName[100]; // 项目名
    char StartDate[11];    // 开始日期
    char EndDate[11];      // 项目终止日期
    struct Project *next;
    struct Project *prev;
    Status status; // 提交状态
    double AddGPA; // 加分
} Project;

typedef struct Award // 设置Award结构体变量
{
    char CompetitionName[100]; // 竞赛名
    char Organizer[100];       // 主办方
    char Winner[32];           // 成员
    char AwardLevel[100];      // 竞赛级别
    char date[11];             // 竞赛日期
    struct Award *next;
    struct Award *prev;
    Status status; // 提交状态加分同上
    double AddGPA;
} Award;

typedef struct Student {
    char name[32];
    int id;              // e.g. 20230501
    int institute_grade; // e.g. 2123
    int major;           // e.g. 2101 for 计算机科学与技术
    struct {
        Essay *essays;
        Project *projects;
        Award *awards;
    } Benefits;
    int failed;         // 不及格课程数
    double GPA_basic;   // 学业成绩
    double GPA_overall; // 学科素质加分分数
    struct {
        int rk;
        int basis;
    } rank;
    Enroll *enrolled;
    char pw_MD5[33];
} Student;

typedef struct Student_Node {
    Student stu;
    struct Student_Node *prev;
    struct Student_Node *next;
} Student_Node;

struct STUMAN_DataAddress {
    Student_Node *pStudentHead;
    Student_Node *pStudentFoot;
    Course_Node *pCourseHead;
    Course_Node *pCourseFoot;
};
extern struct STUMAN_DataAddress data_address;

extern Student_Node *idIndex[90][4][32][32];
extern Student_List *nameIndex[65536];
extern Student_List *gradeIndex[90][4];

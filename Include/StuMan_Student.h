#pragma once
#include "StuMan_Benefit.h"
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
Student_List Benefits_PendingVerified;
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

#pragma once
#include <stdbool.h>
#define NULL 0LL

#ifdef DEBUG
#include <stddef.h>
#define MALLOC my_alloc
#define FREE my_free
typedef struct {
    int cnt;
    void *table[2048];
} MemoryRecord;
extern MemoryRecord memRec;
void *my_alloc(size_t _Size);
void my_free(void *_Memory);
#else
#define MALLOC malloc
#define FREE free
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

typedef struct {
    char name[32];
    int id;              // e.g. 20230501
    int institute_grade; // e.g. 2123
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

extern Student *idIndex[90][4][32][32];
extern Student_List *nameIndex[65536];
extern Student_List *gradeIndex[90][4];

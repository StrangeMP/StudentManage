#pragma once
#include "Instit.h"
#include <stdbool.h>

typedef struct Student_IdNode {
    int id;
    struct Student_IdNode *next;
} Student_IdNode;

// 课程信息
typedef struct Course {
    // 学年学期 e.g. 202301 stands for 2023-2024 学年第一学期
    int semester;
    char name[50];            // 课程名 e.g.大学物理B
    char id[13];              // 课程号 e.g. ae22931102
    char num;                 // 同一课程不同老师用此区分
    char teacher[32];         // 任课教师
    int pub_category;         // 校公选课类别
    int type;                 // 课程类别 e.g. 通识基础课
    char nature;              // 课程性质 1必修/ 2选修/ 3限选
    double credit;            // 学分
    int hours;                // 学时
    char exam_date[11];       // 考试日期 e.g. "2023-06-26"
    char grade_type;          // 1百分制/ 2五级制
    char exam_type;           // 1考试 /2考察
    int institute;               // 开课单位
    Student_IdNode *followed; // 选这门课的学生的学号的链表

    struct Course *next;
} Course;

typedef struct Course_Node {
    Course crs;
    struct Course_Node *next;
} Course_Node;

// 学生选课信息
typedef struct Enroll {
    char course_id[13]; // 课程号 e.g. ae22931102
    union {
        double grade; // 百分制 总成绩
        char level;   // 五级制成绩 1优秀/ 2良好/ 3及格/ 4不及格
    };
    char retake;    // 1初修/ 2重修/ 3重考
    bool major;     // 是否主修
    double gpa;     // 绩点
    bool passed;    // 是否及格
    bool effective; // 是否有效

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
    struct Student_Node *next;
} Student_Node;

// Global pointers to loaded data
// Globally used.
struct {
    Student_Node *pStudentHead;
    Student_Node *pStudentFoot;
    Course_Node *pCourseHead;
    Course_Node *pCourseFoot;
} data_address = {NULL, NULL, NULL, NULL};

// 校公选课类别
const char *pubCourseCategory[] = {"", // 非校公选课
                                   "限选大学生心理健康",
                                   "哲学智慧与品判思维(I)",
                                   "大学生职业发展与就业创业指导",
                                   "文化理解与历史传承(II)",
                                   "经济与社会发展类(III)",
                                   "当代中国与公民责任(III)",
                                   "管理与行为科学类(IV)",
                                   "全球视野与文明交流(IV)",
                                   "艺术鉴赏与审美体验(V)",
                                   "工程人文卓越课程类(VII)",
                                   "科学精神与创新创造(VI)",
                                   "科学与技术类(V)",
                                   "生态环境与生命关怀(VII)",
                                   "人际沟通与合作精神(VIII)",
                                   "文学与艺术类(I)",
                                   "创新与创业类(VI)",
                                   "历史与文化类(II)",
                                   "医学人文卓越课程类(VII)",
                                   "慕课"};

char *courseType[] = {
    "通识教育课",
    "学科基础课",
    "专业教育课",
    "拓展课",
};

// Load data from a binary file.
// Data to load and build includes:
//      1) Student, Course and Awards
//      2) nameIndex and idIndex
void LoadData(const char *fileDir);

typedef struct {
    int student_count;
    Student_IdNode *first;
    Student_IdNode *end;
} Student_List;
void Build_Student_Index(Student *pStu);

Student *Get_Student_by_id(const int id);
Student_IdNode *Get_Students_by_name(const char name[]);
const Student_List Get_StudentList_by_grade(int institute_and_grade);
Course *Get_Course(const char *course_id);

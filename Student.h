#pragma once
#include "Instit.h"
#include <stdbool.h>

// 课程信息
typedef struct Course {
    // 学年学期 e.g. 202301 stands for 2023-2024 学年第一学期
    int semester;
    wchar_t name[15];    // 课程名 e.g.大学物理B
    char id[13];         // 课程号 e.g. ae22931102
    char num;            // 同一课程不同老师用此区分
    wchar_t teacher[10]; // 任课教师
    int pub_category;    // 校公选课类别
    int type;            // 课程类别 e.g. 通识基础课
    char nature;         // 课程性质 1必修/ 2选修/ 3限选
    double credit;       // 学分
    int hours;           // 学时
    char exam_date[11];  // 考试日期 e.g. "2023-06-26"
    char grade_type;     // 1百分制/ 2五级制
    char exam_type;      // 1考试 /2考察
    int instit;          // 开课单位

    struct Course *next;
} Course;

// 学生选课信息
typedef struct Enroll {
    char course_id[13]; // 课程号 e.g. ae22931102
    double grade;       // 总成绩
    char retake;        // 1初修/ 2重修/ 3重考
    bool major;         // 是否主修
    double gpa;         // 绩点
    bool passed;        // 是否及格
    bool effective;     // 是否有效

    struct Enroll *next;
} Enroll;

typedef struct {
    wchar_t name[10];
    int id;                    // e.g. 20230501
    int institute_grade_class; // e.g. 212305
    Enroll *courses;
} Student;

typedef struct {
    Student stu;
    Student *next;
} StuNode;

// 校公选课类别
wchar_t *pubCourseCategory[] = {L"", // 非校公选课
                                L"限选大学生心理健康",
                                L"哲学智慧与品判思维(I)",
                                L"大学生职业发展与就业创业指导",
                                L"文化理解与历史传承(II)",
                                L"经济与社会发展类(III)",
                                L"当代中国与公民责任(III)",
                                L"管理与行为科学类(IV)",
                                L"全球视野与文明交流(IV)",
                                L"艺术鉴赏与审美体验(V)",
                                L"工程人文卓越课程类(VII)",
                                L"科学精神与创新创造(VI)",
                                L"科学与技术类(V)",
                                L"生态环境与生命关怀(VII)",
                                L"人际沟通与合作精神(VIII)",
                                L"文学与艺术类(I)",
                                L"创新与创业类(VI)",
                                L"历史与文化类(II)",
                                L"医学人文卓越课程类(VII)",
                                L"慕课"};

wchar_t *courseType[] = {
    L"通识教育课",
    L"学科基础课",
    L"专业教育课",
    L"拓展课",
};

extern Student *idIndex[90][4][30][30];
extern int nameIndex[65536];

void buildIDIndex(Student *);
size_t Get16BitHash(const wchar_t str[]);
void buildNameIndex(Student *);
Student *findStu_by_ID(const int id);
Student *findStu_by_name(const wchar_t *);
#pragma once
#include "Instit.h"

struct Course;
struct Student {
    wchar_t name[10];
    int id;                    // e.g. 20230501
    int institute_grade_class; // e.g. 212305
    Enroll *courses;
};

struct StuNode {
    Student stu;
    Student *next;
};

// 课程信息
struct Course {
    // 学年学期 e.g. 202301 stands for 2023-2024 学年第一学期
    int semester;
    wchar_t name[15];
    char id[13];        // 课程号 e.g. ae22931102
    int pub_category;   // 校公选课类别
    int type;           // 课程类别 e.g. 通识基础课
    char nature;        // 课程性质 1必修/ 2选修/ 3限选
    double credit;      // 学分
    int hours;          // 学时
    char exam_date[11]; // 考试日期 e.g. "2023-06-26"
    char grade_type;    // 1百分制/ 2五级制
    char exam_type;     // 1考试 /2考察
    int instit;         // 开课单位
};

// 学生选课信息
struct Enroll {
    char course_id[13]; // 课程号 e.g. ae22931102
    double grade;       // 总成绩
    char retake;        // 1初修/ 2重修/ 3重考
    bool major;         // 是否主修
    double gpa;         // 绩点
    bool passed;        // 是否及格
    bool effective;     // 是否有效

    Enroll *next;
};

extern Student *idIndex[];
void buildIDIndex(Student *);
void buildNameIndex(Student *);
Student *findStu_by_name(const wchar_t *);
#pragma once
typedef enum { // 设置枚举变量显示状态
    PENDING,   // 受理
    APPROVED,  // 通过
    DENIED     // 否决
} Status;

typedef enum { ESSAY, PROJECT, AWARDS } Benefit_type;

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

void *Student_AddEssay(int stu_id, Essay *essay);
void *Student_AddProject(int stu_id, Project *project);
void *Student_AddAward(int stu_id, Award *award);

#pragma once
typedef enum
{ // 设置枚举变量显示状态
    PENDING,
    APPROVED,
    DENIED
} Status;
typedef enum
{
    ESSAY,
    PROJECT,
    AWARDS
} Benefit_type;
typedef struct Essays // 设置Essays结构体变量
{
    char Author[5][50];
    char EssayName[100];
    char JournalorMeetingName[100];
    int LaunchDate;
    char other[100]; /*卷/期/论文号/页码范围*/
    char level;
    double AddGPA;
    struct Essays *next;
    struct Essays *prev;
    Status status;
}Essays;
typedef struct Projects // 设置Projec结构体变量
{
    char Member[5][50];
    char GuideTeacher[10];
    int ProjectID;
    char ProjectName[100];
    int StartDate;
    int EndDate;
    struct Projects *next;
    struct Projects *prev;
    Status status;
}Projects;
typedef struct Awards  // 设置Awards结构体变量
{
    char CompetitionName[100];
    char sponsor[100];
    char Name[100];
    char AwardLevel;
    int date;
    struct Awards *next;
    struct Awards *prev;
    Status status;
}Awards;
void* Student_AddBenefits(int stu_id, Benefit_type t, void* benifits);



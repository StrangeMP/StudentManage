#include "StuMan_Export.h"
#include "StuMan_Nouns.h"
#include "StuMan_Search.h"
#include "StuMan_Student.h"
#include <stdio.h>
#include <stdlib.h>

static cJSON *Export_Enroll(cJSON *cjson_Stu, Student *stu) {
    cJSON *cjson_enroll = cJSON_CreateArray();
    Enroll *crt_enrollNode = stu->enrolled;
    while (crt_enrollNode) {
        cJSON *crt_obj = cJSON_CreateObject();
        cJSON_AddStringToObject(crt_obj, "课程号", crt_enrollNode->course_id);
        char tmp_semester[] = "2023-2024-1";
        //                    "0123456789X"
        int tmp_sem = crt_enrollNode->semester;
        tmp_semester[2] = tmp_sem / 100 + '0';
        tmp_semester[3] = tmp_sem / 10 % 10 + '0';
        tmp_semester[7] = ((tmp_semester[3] == '9') ? (tmp_semester[2] + 1) : tmp_semester[2]);
        tmp_semester[8] = ((tmp_semester[3] == '9') ? '0' : tmp_semester[3] + 1);
        tmp_semester[10] = tmp_sem % 10 + '0';
        cJSON_AddStringToObject(crt_obj, "学年学期", tmp_semester);
        cJSON_AddNumberToObject(crt_obj, "总成绩", crt_enrollNode->grade);
        cJSON_AddStringToObject(crt_obj, "重修重考", Enroll_retake[crt_enrollNode->retake]);
        cJSON_AddStringToObject(crt_obj, "是否主修", crt_enrollNode->major ? "是" : "否");
        cJSON_AddNumberToObject(crt_obj, "绩点", crt_enrollNode->gpa);
        cJSON_AddStringToObject(crt_obj, "是否及格", crt_enrollNode->passed ? "是" : "否");
        cJSON_AddStringToObject(crt_obj, "是否有效", crt_enrollNode->effective ? "是" : "否");
        cJSON_AddItemToArray(cjson_enroll, crt_obj);
        crt_enrollNode = crt_enrollNode->next;
    }
    cJSON_AddItemToObject(cjson_Stu, "所选课程", cjson_enroll);
    return cjson_Stu;
}

static void Export_Essay(cJSON *cjson_Benefits, Student *stu) {
    cJSON *cjson_Es = cJSON_CreateArray();
    Essay *crt_essay = stu->Benefits.essays;
    while (crt_essay) {
        cJSON *cjson_essay = cJSON_CreateObject();
        cJSON_AddStringToObject(cjson_essay, "论文名", crt_essay->Title);
        cJSON *cjson_authors = cJSON_CreateArray();
        for (int i = 0; crt_essay->Author[i][0] && i < 5; i++)
            cJSON_AddItemToArray(cjson_authors, cJSON_CreateString(crt_essay->Author[i]));
        cJSON_AddItemToObject(cjson_essay, "作者", cjson_authors);
        cJSON_AddStringToObject(cjson_essay, "期刊或会议名", crt_essay->Journal_Conference_Name);
        cJSON_AddStringToObject(cjson_essay, "发表日期", crt_essay->PubDate);
        cJSON_AddStringToObject(cjson_essay, "级别", crt_essay->Classification);
        cJSON_AddNumberToObject(cjson_essay, "加分", crt_essay->AddGPA);
        cJSON_AddStringToObject(cjson_essay, "审核状态", Benefit_Status[crt_essay->status]);
        cJSON_AddItemToArray(cjson_Es, cjson_essay);
        crt_essay = crt_essay->next;
    }
    cJSON_AddArrayToObject(cjson_Es, "论文");
}

static void Export_Project(cJSON *cjson_Benefits, Student *stu) {
    cJSON *cjson_Ps = cJSON_CreateArray();
    Project *crt_project = stu->Benefits.projects;
    while (crt_project) {
        cJSON *cjson_project = cJSON_CreateObject();
        cJSON_AddStringToObject(cjson_project, "项目名称", crt_project->ProjectName);
        cJSON_AddStringToObject(cjson_project, "项目编号", crt_project->ProjectID);
        cJSON *cjson_authors = cJSON_CreateArray();
        for (int i = 0; crt_project->Member[i][0] && i < 5; i++)
            cJSON_AddItemToArray(cjson_authors, cJSON_CreateString(crt_project->Member[i]));
        cJSON_AddItemToObject(cjson_project, "成员", cjson_authors);
        cJSON_AddStringToObject(cjson_project, "指导老师", crt_project->GuideTeacher);
        cJSON_AddStringToObject(cjson_project, "立项时间", crt_project->StartDate);
        cJSON_AddStringToObject(cjson_project, "结项时间", crt_project->EndDate);
        cJSON_AddNumberToObject(cjson_project, "加分", crt_project->AddGPA);
        cJSON_AddStringToObject(cjson_project, "审核状态", Benefit_Status[crt_project->status]);
        cJSON_AddItemToArray(cjson_Ps, cjson_project);
        crt_project = crt_project->next;
    }
    cJSON_AddArrayToObject(cjson_Ps, "项目");
}

static void Export_Award(cJSON *cjson_Benefits, Student *stu) {
    cJSON *cjson_As = cJSON_CreateArray();
    Award *crt_award = stu->Benefits.awards;
    while (crt_award) {
        cJSON *cjson_award = cJSON_CreateObject();
        cJSON_AddStringToObject(cjson_award, "竞赛名称", crt_award->CompetitionName);
        cJSON_AddStringToObject(cjson_award, "主办单位", crt_award->Organizer);
        cJSON_AddStringToObject(cjson_award, "获奖等级", crt_award->AwardLevel);
        cJSON_AddStringToObject(cjson_award, "获奖时间", crt_award->date);
        cJSON_AddNumberToObject(cjson_award, "加分", crt_award->AddGPA);
        cJSON_AddStringToObject(cjson_award, "审核状态", Benefit_Status[crt_award->status]);
        cJSON_AddItemToArray(cjson_As, cjson_award);
        crt_award = crt_award->next;
    }
    cJSON_AddArrayToObject(cjson_As, "竞赛获奖");
}

static void Export_Benefits(cJSON *cjson_Stu, Student *stu) {
    cJSON *cjson_Benefits = cJSON_CreateObject();
    Export_Essay(cjson_Benefits, stu);
    Export_Project(cjson_Benefits, stu);
    Export_Award(cjson_Benefits, stu);
    cJSON_AddItemToObject(cjson_Stu, "素质加分", cjson_Benefits);
}

static cJSON *Student_Export(cJSON *_dest, int stu_id) {
    Student *stu = Get_Student_by_id(stu_id);
    cJSON *cjson_Stu = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_Stu, "姓名", stu->name);
    cJSON_AddNumberToObject(cjson_Stu, "学号", stu->id);
    cJSON_AddStringToObject(cjson_Stu, "学院", Institutes[stu->institute_grade / 100]);
    cJSON_AddStringToObject(cjson_Stu, "专业", Professions[stu->major / 100][stu->major % 100]);
    cJSON_AddNumberToObject(cjson_Stu, "年级", stu->institute_grade % 100 + 2000);
    Export_Enroll(cjson_Stu, stu);
    Export_Benefits(cjson_Stu, stu);
    cJSON_AddItemToArray(cJSON_GetObjectItem(_dest, "学生"), cjson_Stu);
    return _dest;
}

static cJSON *Course_Export(cJSON *_dest, const char *course_id) {
    Course *crs = Get_Course(course_id);
    cJSON *cjson_Crs = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_Crs, "上课老师", crs->teacher);
    cJSON_AddStringToObject(cjson_Crs, "课程名", crs->name);
    cJSON_AddStringToObject(cjson_Crs, "课程号", crs->id);
    cJSON_AddStringToObject(cjson_Crs, "校公选课类别", pubCourseCategory[crs->pub_category]);
    cJSON_AddStringToObject(cjson_Crs, "课程类别", Course_type[crs->type]);
    cJSON_AddStringToObject(cjson_Crs, "课程性质", Course_nature[crs->nature]);
    cJSON_AddStringToObject(cjson_Crs, "显示等级成绩类型", Course_grade_type[crs->grade_type]);
    cJSON_AddStringToObject(cjson_Crs, "开课单位", Institutes[crs->institute]);
    cJSON_AddStringToObject(cjson_Crs, "考试日期", crs->exam_date);
    cJSON_AddStringToObject(cjson_Crs, "考试类型", Course_exam_type[crs->exam_type]);
    cJSON_AddNumberToObject(cjson_Crs, "学时", crs->hours);
    cJSON_AddNumberToObject(cjson_Crs, "学分", crs->credit);
    cJSON *followList = cJSON_CreateArray();
    Student_IdNode *crt_follow = crs->followed->first;
    while (crt_follow) {
        cJSON_AddItemToArray(followList, cJSON_CreateNumber(crt_follow->id));
        crt_follow = crt_follow->next;
    }
    cJSON_AddItemToObject(cjson_Crs, "选课学生学号", followList);
    cJSON_AddItemToArray(cJSON_GetObjectItem(_dest, "课程"), cjson_Crs);
    return _dest;
}

void ExportData(cJSON *_data) {
    cJSON *_dest = cJSON_CreateObject();
    cJSON_AddArrayToObject(_dest, "学生");
    cJSON_AddArrayToObject(_dest, "课程");
    cJSON *ex_Stus = cJSON_GetObjectItem(_data, "Students");
    if (ex_Stus) {
        int stu_num = cJSON_GetArraySize(ex_Stus);
        for (int i = 0; i < stu_num; i++)
            Student_Export(_dest, cJSON_GetArrayItem(ex_Stus, i)->valueint);
    }
    cJSON *ex_Crs = cJSON_GetObjectItem(_data, "Courses");
    if (ex_Crs) {
        int crs_num = cJSON_GetArraySize(ex_Crs);
        for (int i = 0; i < crs_num; i++)
            Course_Export(_dest, cJSON_GetArrayItem(ex_Crs, i)->valuestring);
    }
    FILE *pf = fopen("ExportedData.json", "w");
    char *output = cJSON_Print(_dest);
    fputs(output, pf);
    fclose(pf);
    cJSON_Delete(_dest);
    cJSON_Delete(_data);
    free(output);
}
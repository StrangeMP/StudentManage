#include "StuMan_Export.h"
#include "StuMan_Nouns.h"
#include "StuMan_Search.h"
#include "StuMan_Student.h"
#include <stdio.h>
#include <stdlib.h>

static cJSON *Export_Enroll_To_Student(cJSON *cjson_Stu, Student *stu) {
    cJSON *cjson_enroll = cJSON_CreateArray();
    Enroll *crt_enrollNode = stu->enrolled;
    while (crt_enrollNode) {
        cJSON *crt_obj = cJSON_CreateObject();
        cJSON_AddStringToObject(crt_obj, "课程号", crt_enrollNode->course_id);
        char tmp_semester[] = "2023-2024-1";
        //                    "0123456789X"
        int tmp_sem = crt_enrollNode->semester;
        tmp_semester[2] = tmp_sem / 1000 + '0';
        tmp_semester[3] = tmp_sem / 100 % 10 + '0';
        tmp_semester[7] = ((tmp_semester[3] == '9') ? (tmp_semester[2] + 1) : tmp_semester[2]);
        tmp_semester[8] = ((tmp_semester[3] == '9') ? '0' : tmp_semester[3] + 1);
        tmp_semester[10] = tmp_sem % 10 + '0';
        cJSON_AddStringToObject(crt_obj, "学年学期", tmp_semester);
        if (Get_Course(crt_enrollNode->course_id)->grade_type == 1)
            cJSON_AddNumberToObject(crt_obj, "总成绩", crt_enrollNode->grade);
        else
            cJSON_AddStringToObject(crt_obj, "总成绩", Enroll_level[crt_enrollNode->level]);
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

static cJSON *Student_Export(cJSON *_dest, int stu_id) {
    Student *stu = Get_Student_by_id(stu_id);
    cJSON *cjson_Stu = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_Stu, "姓名", stu->name);
    cJSON_AddNumberToObject(cjson_Stu, "学号", stu->id);
    cJSON_AddStringToObject(cjson_Stu, "学院", Institutes[stu->institute_grade / 100]);
    cJSON_AddStringToObject(cjson_Stu, "专业", Professions[stu->major / 100][stu->major % 100]);
    cJSON_AddNumberToObject(cjson_Stu, "年级", stu->institute_grade % 100 + 2000);
    Export_Enroll_To_Student(cjson_Stu, stu);
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
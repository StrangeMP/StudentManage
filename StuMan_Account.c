#include "StuMan_Account.h"
#include "StuMan_Search.h"
#include "StuMan_Student.h"
#include "VECTOR.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

VECTOR(TeacherVector, struct Teacher)
TeacherVector *t_vec = NULL;

const char *Get_Student_PWMD5(int stu_id) { return Get_Student_by_id(stu_id)->pw_MD5; }

static struct Teacher *Add_Teacher(cJSON *info) {
    if (t_vec == NULL)
        t_vec = TeacherVector_create();
    struct Teacher crt_teacher = {{'\0'}, {'\0'}, {'\0'}, NULL};
    strcpy(crt_teacher.name, cJSON_GetObjectItem(info, "name")->valuestring);
    strcpy(crt_teacher.id, cJSON_GetObjectItem(info, "id")->valuestring);
    char *pw = cJSON_GetObjectItem(info, "PW_MD5")->valuestring;
    if (strlen(pw) == 32)
        strcpy(crt_teacher.PW_MD5, pw);
    StrVec *crses = StrVec_create();
    Course_Node *crt_crs = data_address.pCourseHead;
    while (crt_crs) {
        if (!strcmp(crt_crs->crs.teacher, crt_teacher.name))
            StrVec_push_back(crses, crt_crs->crs.id);
        crt_crs = crt_crs->next;
    }
    crt_teacher.courses = crses;
    TeacherVector_push_back(t_vec, crt_teacher);
    return TeacherVector_end(t_vec) - 1;
}

void Teacher_Signup(const char *fileDir) {
    if (fileDir == NULL) {
        printf("ImportData Error: invalid fileDir\n");
        return;
    }
    FILE *pf = fopen(fileDir, "r");
    fseek(pf, 0, SEEK_END);
    long fsize = ftell(pf);
    fseek(pf, 0, SEEK_SET);
    char *rawData = (char *)MALLOC(fsize + 1);
    fread(rawData, fsize, 1, pf);
    rawData[fsize] = '\0';
    fclose(pf);
    cJSON *tea_list = cJSON_Parse(rawData);
    FREE(rawData);
    int tea_cnt = cJSON_GetArraySize(tea_list);
    for (int i = 0; i < tea_cnt; i++)
        Add_Teacher(cJSON_GetArrayItem(tea_list, i));
}

static const char *comparator = NULL;
static bool comp(TeacherVector_iterator item) {
    if (strcmp(item->id, comparator) == 0)
        return true;
    else
        return false;
}
struct Teacher *Get_Teacher(const char *teacher_id) {
    if (t_vec == NULL)
        return NULL;
    comparator = teacher_id;
    return TeacherVector_find(t_vec, comp);
}

void CleanTeachers() {
    if (t_vec == NULL)
        return;
    TeacherVector_iterator iter = TeacherVector_begin(t_vec);
    TeacherVector_iterator end = TeacherVector_end(t_vec);
    for (; iter != end; iter++) {
        StrVec_destroy(iter->courses);
    }
    TeacherVector_destroy(t_vec);
}
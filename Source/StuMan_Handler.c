#include "StuMan_Account.h"
#include "StuMan_Delete.h"
#include "StuMan_Export.h"
#include "StuMan_Import.h"
#include "StuMan_Nouns.h"
#include "StuMan_Search.h"
#include "StuMan_Memory.h"
#include "StuMan_Statistics.h"
#include "StuMan_Student.h"
#include "VECTOR.h"
#include "cJSON.h"
#include "string.h"
#include <ctype.h>
#include <stdio.h>

// index of the same request in the two arrays should be identical
enum REQ_T {
    GET_STU_PRO,
    GET_CRS_TEACHER,
    GET_CRS_ID,
    ADD,
    DEL_STU,
    DEL_STU_ENR,
    GET_STU_ID,
    GET_STU_NAME,
    GET_ENR_BY_CRSID,
    LOGIN,
    GET_PENDING,
    DEL_CRS,
    GET_CRS_NAME
};
static const char *REQ_STR[] = {"get_students_by_profession",
                                "get_courses_by_teacher",
                                "get_courses_by_course_id",
                                "add",
                                "delete_students",
                                "delete_student_enrolls",
                                "get_students_by_id",
                                "get_students_by_name",
                                "get_student_enrolls_by_course_id",
                                "login",
                                "get_pending_list",
                                "del_courses",
                                "get_courses_by_course_names"};

static void AddResponse(cJSON *_dest, cJSON *_item, int num) {
    cJSON *response = cJSON_CreateObject();
    cJSON_AddItemToObject(response, "Number", cJSON_CreateNumber(num));
    cJSON_AddItemToObject(response, "response", _item);
    cJSON_AddItemToArray(_dest, response);
}

static void Handle_GET_STU_PRO(cJSON *response, cJSON *req) {
    Student **stu_arr = NULL;
    size_t stu_cnt =
        Extract_Students(&stu_arr, cJSON_GetObjectItem(req, "institute_grade")->valueint,
                         cJSON_GetObjectItem(req, "major")->valueint);
    int *stuid_arr = (int *)MALLOC(sizeof(int) * stu_cnt);
    for (size_t i = 0; i < stu_cnt; i++)
        stuid_arr[i] = stu_arr[i]->id;
    AddResponse(response, ExportData(CreateExportList(stuid_arr, stu_cnt, NULL, 0), NULL),
                cJSON_GetObjectItem(req, "Number")->valueint);
    FREE(stuid_arr);
}

VECTOR(CrsIdVec, const char *)
static void Handle_GET_CRS_TEACHER(cJSON *reponse, cJSON *req) {
    Course_Node *crt_crs = data_address.pCourseHead;
    const char *teacher = cJSON_GetObjectItem(req, "teacher")->valuestring;
    CrsIdVec *vec = CrsIdVec_create();
    while (crt_crs) {
        if (!strcmp(crt_crs->crs.teacher, teacher))
            CrsIdVec_push_back(vec, crt_crs->crs.id);
        crt_crs = crt_crs->next;
    }
    AddResponse(reponse,
                ExportData(CreateExportList(NULL, 0, CrsIdVec_data(vec), CrsIdVec_size(vec)), NULL),
                cJSON_GetObjectItem(req, "Number")->valueint);
    CrsIdVec_destroy(vec);
}

static void Handle_GET_CRS_ID(cJSON *response, cJSON *req) {
    cJSON *crs_id_collect = cJSON_GetObjectItem(req, "course_ids");
    size_t crs_cnt = cJSON_GetArraySize(crs_id_collect);
    if (crs_cnt == 0)
        return;
    const char **crs_arr = (const char **)MALLOC(sizeof(const char *) * crs_cnt);
    for (size_t i = 0; i < crs_cnt; i++)
        crs_arr[i] = cJSON_GetArrayItem(crs_id_collect, i)->valuestring;
    AddResponse(response, ExportData(CreateExportList(NULL, 0, crs_arr, crs_cnt), NULL),
                cJSON_GetObjectItem(req, "Number")->valueint);
    FREE(crs_arr);
}

static void Handle_ADD(cJSON *response, cJSON *req) {
    const char *data = cJSON_Print(cJSON_GetObjectItem(req, "data"));
    ImportData_fromString(data);
    free((void *)data); // memory allocated by cJSON, do not use FREE
    AddResponse(response, cJSON_CreateBool(true), cJSON_GetObjectItem(req, "Number")->valueint);
}

static void Handle_DEL_STU(cJSON *response, cJSON *req) {
    cJSON *del_arr = cJSON_GetObjectItem(req, "student_ids");
    int del_cnt = cJSON_GetArraySize(del_arr);
    for (int i = 0; i < del_cnt; i++)
        del(STU, 1, cJSON_GetArrayItem(del_arr, i)->valueint);
    AddResponse(response, cJSON_CreateBool(true), cJSON_GetObjectItem(req, "Number")->valueint);
}

static void Handle_GET_STU_ID(cJSON *response, cJSON *req) {
    cJSON *cjson_stu_arr = cJSON_GetObjectItem(req, "student_ids");
    int stu_cnt = cJSON_GetArraySize(cjson_stu_arr);
    int *stu_arr = (int *)MALLOC(sizeof(int) * stu_cnt);
    for (int i = 0; i < stu_cnt; i++)
        stu_arr[i] = cJSON_GetArrayItem(cjson_stu_arr, i)->valueint;
    AddResponse(response, ExportData(CreateExportList(stu_arr, stu_cnt, NULL, 0), NULL),
                cJSON_GetObjectItem(req, "Number")->valueint);
    FREE(stu_arr);
}

static void Handle_GET_STU_NAME(cJSON *response, cJSON *req) {
    Student_List *stu_list = Get_StudentList_by_name(cJSON_GetObjectItem(req, "name")->valuestring);
    int *stu_arr = (int *)MALLOC(sizeof(int) * stu_list->student_count);
    Student_IdNode *crt_node = stu_list->first;
    for (int i = 0; crt_node; i++) {
        stu_arr[i] = crt_node->id;
        crt_node = crt_node->next;
    }
    AddResponse(response,
                ExportData(CreateExportList(stu_arr, stu_list->student_count, NULL, 0), NULL),
                cJSON_GetObjectItem(req, "Number")->valueint);
    FREE(stu_arr);
}

#define HTTP_HEADER_PROTO                                                                          \
    "HTTP/1.1 200 OK\r\n"                                                                          \
    "Content-Type: text/plain\r\n"                                                                 \
    "Content-Length: \r\n"                                                                         \
    "Access-Control-Allow-Origin: *\r\n"                                                           \
    "Connection: close\r\n\r\n"
#define HEADER_LEN sizeof(HTTP_HEADER_PROTO) / sizeof(char) - 1

// Function to create an HTTP response with the specified content
static char *create_http_response(const char *content) {
    size_t content_length = strlen(content);
    char lenlen[10];
    sprintf(lenlen, "%zu", content_length);
    size_t buffer_size = HEADER_LEN + strlen(lenlen) + content_length + 1;
    char *response = (char *)MALLOC(sizeof(char) * buffer_size);
    snprintf(response, buffer_size,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %zu\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "Connection: close\r\n\r\n"
             "%s",
             content_length, content);
    FREE((void *)content);
    return response;
}

static void Handle_DEL_STU_ENR(cJSON *response, cJSON *req) {
    cJSON *del_list = cJSON_GetObjectItem(req, "info");
    int del_cnt = cJSON_GetArraySize(del_list);
    for (int i = 0; i < del_cnt; i++) {
        cJSON *crt_del = cJSON_GetArrayItem(del_list, i);
        int stu_id = cJSON_GetObjectItem(crt_del, "student_id")->valueint;
        cJSON *enr_arr = cJSON_GetObjectItem(crt_del, "course_ids");
        int enr_cnt = cJSON_GetArraySize(enr_arr);
        for (int i = 0; i < enr_cnt; i++)
            del(CRS_OF_STU, 2, stu_id, cJSON_GetArrayItem(enr_arr, i)->valuestring);
    }
    AddResponse(response, cJSON_CreateBool(true), cJSON_GetObjectItem(req, "Number")->valueint);
}

// 1教务 2老师 3学生
static void Handle_LOGIN(cJSON *response, cJSON *req) {
    char *un = cJSON_GetObjectItem(req, "username")->valuestring;
    int identity = isalpha(un[0]) ? 2 : 3;
    cJSON *acc_info = cJSON_CreateObject();
    switch (identity) {
    case 1:
        break;
    case 2: {
        struct Teacher *crt_tea = Get_Teacher(cJSON_GetObjectItem(req, "username")->valuestring);
        cJSON_AddItemToObject(acc_info, "username",
                              crt_tea ? cJSON_CreateString(crt_tea->id) : cJSON_CreateString(""));
        if (crt_tea) {
            cJSON_AddItemToObject(acc_info, "password", cJSON_CreateString(crt_tea->PW_MD5));
            cJSON_AddItemToObject(acc_info, "identity", cJSON_CreateNumber(2));
            cJSON_AddItemToObject(acc_info, "name", cJSON_CreateString(crt_tea->name));
        }
        break;
    }
    case 3: {
        int stu_id = 0;
        sscanf(cJSON_GetObjectItem(req, "username")->valuestring, "%d", &stu_id);
        Student *crt_stu = Get_Student_by_id(stu_id);
        cJSON_AddItemToObject(acc_info, "username",
                              crt_stu ? cJSON_CreateNumber(crt_stu->id) : cJSON_CreateNumber(0));
        if (crt_stu) {
            cJSON_AddItemToObject(acc_info, "password", cJSON_CreateString(crt_stu->pw_MD5));
            cJSON_AddItemToObject(acc_info, "identity", cJSON_CreateNumber(3));
            cJSON_AddItemToObject(acc_info, "name", cJSON_CreateString(crt_stu->name));
        }
        break;
    }
    default:
        break;
    }
    AddResponse(response, acc_info, cJSON_GetObjectItem(req, "Number")->valueint);
}

VECTOR(intVec, int)
static void Handle_GET_ENR_BY_CRSID(cJSON *response, cJSON *req) {
    intVec *ivec = intVec_create();
    cJSON *cjson_crses = cJSON_GetObjectItem(req, "course_ids");
    int crs_cnt = cJSON_GetArraySize(cjson_crses);
    for (int i = 0; i < crs_cnt; i++) {
        Course *crt_crs = Get_Course(cJSON_GetArrayItem(cjson_crses, i)->valuestring);
        if (crt_crs == NULL)
            return;
        Student_IdNode *crt_node = crt_crs->followed->first;
        while (crt_node) {
            intVec_push_back(ivec, crt_node->id);
            crt_node = crt_node->next;
        }
    }
    AddResponse(response,
                ExportData(CreateExportList(intVec_data(ivec), intVec_size(ivec), NULL, 0), NULL),
                cJSON_GetObjectItem(req, "Number")->valueint);
    intVec_destroy(ivec);
}

static void Handle_GET_PENDING(cJSON *response, cJSON *req) {
    intVec *ivec = intVec_create();
    Student_IdNode *node = Benefits_PendingVerified->first;
    while (node) {
        intVec_push_back(ivec, node->id);
        node = node->next;
    }
    AddResponse(response,
                ExportData(CreateExportList(intVec_data(ivec), intVec_size(ivec), NULL, 0), NULL),
                cJSON_GetObjectItem(req, "Number")->valueint);
    intVec_destroy(ivec);
}

static void Handle_DEL_CRS(cJSON *response, cJSON *req) {
    cJSON *del_queue = cJSON_GetObjectItem(req, "course_ids");
    int del_num = cJSON_GetArraySize(del_queue);
    for (int i = 0; i < del_num; i++)
        del(CRS, 1, cJSON_GetArrayItem(del_queue, i)->valuestring);
    AddResponse(response, cJSON_CreateBool(true), cJSON_GetObjectItem(req, "Number")->valueint);
}

static void Handle_GET_CRS_NAME(cJSON *response, cJSON *req) {
    cJSON *crs_name_collect = cJSON_GetObjectItem(req, "course_names");
    size_t crs_cnt = cJSON_GetArraySize(crs_name_collect);
    if (crs_cnt == 0)
        return;
    const char **crs_arr = (const char **)MALLOC(sizeof(const char *) * crs_cnt);
    for (size_t i = 0; i < crs_cnt; i++) {
        char *crs_name = cJSON_GetArrayItem(crs_name_collect, i)->valuestring;
        for (Course_Node *crt_crs = data_address.pCourseHead; crt_crs; crt_crs = crt_crs->next) {
            if (!strcmp(crt_crs->crs.name, crs_name)) {
                crs_arr[i] = crt_crs->crs.id;
                break;
            }
        }
    }
    AddResponse(response, ExportData(CreateExportList(NULL, 0, crs_arr, crs_cnt), NULL),
                cJSON_GetObjectItem(req, "Number")->valueint);
    FREE(crs_arr);
}

char *Handler(const char *reqs) {
    if (!reqs)
        return NULL;
    cJSON *all_reqs = cJSON_Parse(reqs);
    if (!cJSON_IsArray(all_reqs))
        return NULL;
    int req_num = cJSON_GetArraySize(all_reqs);
    if (!req_num) {
        cJSON_Delete(all_reqs);
        return NULL;
    }
    cJSON *response = cJSON_CreateArray();
    for (int i = 0; i < req_num; i++) {
        cJSON *crt_req = cJSON_GetArrayItem(all_reqs, i);
        cJSON *cjson_req_t = cJSON_GetObjectItem(crt_req, "request");
        if (!cjson_req_t)
            continue;
        switch ((enum REQ_T)getNounIndex(REQ_STR, sizeof(REQ_STR) / sizeof(const char *),
                                         cjson_req_t->valuestring)) {
        case GET_STU_PRO:
            Handle_GET_STU_PRO(response, crt_req);
            break;
        case GET_CRS_TEACHER:
            Handle_GET_CRS_TEACHER(response, crt_req);
            break;
        case GET_CRS_ID:
            Handle_GET_CRS_ID(response, crt_req);
            break;
        case ADD:
            Handle_ADD(response, crt_req);
            break;
        case DEL_STU:
            Handle_DEL_STU(response, crt_req);
            break;
        case DEL_STU_ENR:
            Handle_DEL_STU_ENR(response, crt_req);
            break;
        case GET_STU_ID:
            Handle_GET_STU_ID(response, crt_req);
            break;
        case GET_STU_NAME:
            Handle_GET_STU_NAME(response, crt_req);
            break;
        case LOGIN:
            Handle_LOGIN(response, crt_req);
            break;
        case GET_ENR_BY_CRSID:
            Handle_GET_ENR_BY_CRSID(response, crt_req);
            break;
        case GET_PENDING:
            Handle_GET_PENDING(response, crt_req);
            break;
        case DEL_CRS:
            Handle_DEL_CRS(response, crt_req);
            break;
        case GET_CRS_NAME:
            Handle_GET_CRS_NAME(response, crt_req);
            break;
        default:
            break;
        }
    }
    cJSON_Delete(all_reqs);
    char *json_result = cJSON_Print(response);
    cJSON_Delete(response);
    char *ret = create_http_response(json_result);
    return ret;
}

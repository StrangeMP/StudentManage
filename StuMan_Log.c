#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "cJSON.h"
#include "StuMan_Log.h"

#define ADD_STR2OBJ(output,cjson_Data,key,value){\
    cJSON_AddStringToObject(output,key,\
                        cJSON_GetStringValue(\
                        cJSON_GetObjectItem (\
                        cjson_Data,value))) ;\
}

#define ADD_NUM2OBJ(output,cjson_Data,key,value){\
    cJSON_AddNumberToObject(output,key,\
                            cJSON_GetNumberValue(\
                            cJSON_GetObjectItem (\
                            cjson_Data,value))) ;\
}

#define ARRAY_CPY(array,srcar){\
    int size = cJSON_GetArraySize(srcar);\
    for(int i = 0;i < size;i++)\
    {\
        cJSON_AddItemToArray(array,\
                        cJSON_CreateString(\
                        cJSON_GetStringValue(\
                        cJSON_GetArrayItem(\
                        srcar,i))));\
    }\
}


void Log(const char *request)
{
    cJSON *cjson_Data = cJSON_Parse(request);
    cJSON *cjson_root;
    cJSON *output = cJSON_CreateObject();
    time_t rawtime;
    struct tm *t;
    long len;
    char *raw;

    //获取文件数据
    FILE* f = fopen("log.json","r");
    if (f == NULL) 
    {
        printf("Open file fail\n");
        cJSON_Delete(cjson_root);
        cJSON_Delete(cjson_Data);
        cJSON_Delete(output);
        fclose(f);
        return;
    } 
    
    fseek(f,0,SEEK_END);
    len = ftell(f);
    fseek(f,0,SEEK_SET);
    
    if(len == 0) cjson_root = cJSON_CreateObject();
    else 
    {
        raw = (char *)malloc(sizeof(char) * len + 1);
        memset(raw, 0, len + 1);
        fread(raw,sizeof(char),len,f);
        cjson_root = cJSON_Parse(raw);
    }

    fclose(f);

    //获取操作的类型
    int num =   cJSON_GetNumberValue(
                cJSON_GetObjectItem(
                cjson_Data,"Number"));
    //记录操作的内容;
    ADD_STR2OBJ(output,cjson_Data,"操作","request");

    switch(num)
    {
        case 1://get_students_by_profession
        ADD_NUM2OBJ(output,cjson_Data,"学院年级","institute_grade");
        ADD_NUM2OBJ(output,cjson_Data,"主修","major");
        break;

        case 2://get_courses_by_teacher
        ADD_STR2OBJ(output,cjson_Data,"老师","teacher");
        break;

        case 3://get_courses_by_course_id
        {
        cJSON* array = cJSON_CreateArray();
        cJSON* srcar = cJSON_GetObjectItem(cjson_Data,"course_ids");
        ARRAY_CPY(array,srcar);
        cJSON_AddItemToObject(output,"课程id",array); 
        break;
        }
        case 4://add
        {
        cJSON* data = cJSON_CreateObject();
        cJSON* stu= cJSON_CreateArray();
        cJSON* crs= cJSON_CreateArray();
        cJSON* src = cJSON_GetObjectItem(cjson_Data,"data");
        cJSON* stusrc= cJSON_GetObjectItem(src,"学生");
        cJSON* crssrc= cJSON_GetObjectItem(src,"课程");
        ARRAY_CPY(stu,stusrc);
        ARRAY_CPY(crs,crssrc);
        cJSON_AddItemToObject(data,"学生",stu);
        cJSON_AddItemToObject(data,"课程",crs);
        cJSON_AddItemToObject(output,"数据",data);        
        break;
        }

        case 5://delete_students
        {
        cJSON* array = cJSON_CreateArray();
        cJSON* srcar = cJSON_GetObjectItem(cjson_Data,"student_ids");
        ARRAY_CPY(array,srcar);
        cJSON_AddItemToObject(output,"学生id",array);
        break;
        }
        case 6://delete_student_enrolls              
        {
        cJSON* info = cJSON_CreateArray();
        cJSON* src = cJSON_GetObjectItem(cjson_Data,"info");
        int size = cJSON_GetArraySize(src);
        cJSON* subinfo[size];
        cJSON* crs[size];
        cJSON* srcj;
        
        for(int j = 0;j < size;j++)
        {
            crs[j] = cJSON_CreateArray();
            subinfo[j] = cJSON_CreateObject();
            srcj = cJSON_GetArrayItem(src,j);
            ADD_NUM2OBJ(subinfo[j],srcj,"学生id","student_id");
            cJSON* crsrc = cJSON_GetObjectItem(srcj,"course_ids");
            ARRAY_CPY(crs[j],crsrc);
            cJSON_AddItemToObject(subinfo[j],"课程id",crs[j]);
            cJSON_AddItemToArray(info,subinfo[j]);
        }
        cJSON_AddItemToObject(output,"信息",info);           
        break;
        }

        case 7://get_students_by_id
        {
        cJSON* array = cJSON_CreateArray();
        cJSON* srcar = cJSON_GetObjectItem(cjson_Data,"student_ids");
        ARRAY_CPY(array,srcar);
        cJSON_AddItemToObject(output,"学生id",array);
        break;
        }
        case 8://get_students_by_name
        ADD_STR2OBJ(output,cjson_Data,"名字","name");
        break;

        case 9://login
        ADD_STR2OBJ(output,cjson_Data,"用户名","username");
        ADD_NUM2OBJ(output,cjson_Data,"身份"  ,"identity");
        break;

        case 10://get_student_enrolls_by_course_id
        {
        cJSON* array = cJSON_CreateArray();
        cJSON* srcar = cJSON_GetObjectItem(cjson_Data,"course_ids");
        ARRAY_CPY(array,srcar);
        cJSON_AddItemToObject(output,"课程id",array);
        break;
        }

        case 11:break;//get_pending_list
    }

    
    //记录操作的时间
    time(&rawtime);
    t = localtime(&rawtime); 
    cJSON_AddItemToObject(cjson_root,
                          strtok(asctime(t),"\n"),output);
    //写入log文件
    char* out = cJSON_Print(cjson_root);
    FILE* fp = fopen("log.json","w");
    fwrite(out,sizeof(char),strlen(out),fp);

    //释放内存
    fclose(fp);
    if(len!=0) free(raw);
    cJSON_Delete(cjson_Data);
    cJSON_Delete(cjson_root);
    
    
    return;
}
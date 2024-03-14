#include "StuMan_BuildIndex.h"
#include "StuMan_Node.h"
#include "StuMan_Search.h"
#include "StuMan_Student.h"

#define STU_MAN_YEAR 24
static void Build_ID_Index(Student *pStu) {
    int id = pStu->id;
    idIndex[id / 1000000][id / 10000 % 100][id / 100 % 100][id % 100] = pStu;
}

static void Build_Name_Index(Student *pStu) {
    // try fetch the name from index
    Student_List *stu_list = nameIndex[Get_16bit_Hash(pStu->name)];
    if (stu_list == NULL) {
        nameIndex[Get_16bit_Hash(pStu->name)] = Student_List_AddStudentID(stu_list, pStu->id);
    } else
        Student_List_AddStudentID(stu_list, pStu->id);
}

static void Build_Grade_Index(Student *pStu) {
    int institute = pStu->institute_grade / 100;
    int grade = STU_MAN_YEAR - pStu->institute_grade % 100 - 1;
    Student_List *stu_list = gradeIndex[institute][grade];
    if (stu_list == NULL)
        gradeIndex[institute][grade] = Student_List_AddStudentID(NULL, pStu->id);
    else
        Student_List_AddStudentID(stu_list, pStu->id);
}

void Build_Student_Index(Student *pStu) {
    if (Get_Student_by_id(pStu->id) == NULL) {
        Build_ID_Index(pStu);
        Build_Name_Index(pStu);
        Build_Grade_Index(pStu);
    }
}
#include "StuMan_Statistics.h"
#include "StuMan_Nouns.h"
#include "StuMan_Search.h"
#include "StuMan_Student.h"
#include <stdio.h>
#include <stdlib.h>

#define COMPARE_EXPAND(X)                                                                          \
    static int Student_Compare_##X(Student *stu1, Student *stu2) {                                 \
        return Student_Compare_By_Property(stu1, stu2, #X);                                        \
    }
#define Student_Compare(X) Student_Compare_##X

static int Student_Failed_Cnt(Student *stu) {
    Enroll *enr_node = stu->enrolled;
    int cnt = 0;
    while (enr_node) {
        if (!enr_node->passed)
            ++cnt;
        enr_node = enr_node->next;
    }
    stu->failed = cnt;
    return cnt;
}

static double Student_CalcGPAbasic(Student *stu) {
    Enroll *enr_node = stu->enrolled;
    double total_points = 0.0;
    int total_credits = 0;
    while (enr_node) {
        Course *crt_Crs = Get_Course(enr_node->course_id);
        total_points += enr_node->gpa * crt_Crs->credit;
        total_credits += crt_Crs->credit;
        enr_node = enr_node->next;
    }
    double gpa = total_points / total_credits;
    stu->GPA_basic = gpa;
    return gpa;
}

static double Student_CalcGPAoverall(Student *stu) {
    double extra_gpa = 0.0;
    Essay *crt_essay = stu->Benefits.essays;
    Project *crt_proj = stu->Benefits.projects;
    Award *crt_awards = stu->Benefits.awards;
    while (crt_essay) {
        extra_gpa += crt_essay->AddGPA;
        crt_essay = crt_essay->next;
    }
    while (crt_proj) {
        extra_gpa += crt_proj->AddGPA;
        crt_proj = crt_proj->next;
    }
    while (crt_awards) {
        extra_gpa += crt_awards->AddGPA;
        crt_awards = crt_awards->next;
    }
    return stu->GPA_overall = Student_CalcGPAbasic(stu) + (extra_gpa > 0.4 ? 0.4 : extra_gpa);
}

// 不及格课程数、学业成绩、学科素质加分分数
void Student_Update(int stu_id) {
    Student *stu = Get_Student_by_id(stu_id);
    Student_Failed_Cnt(stu);
    Student_CalcGPAbasic(stu);
    Student_CalcGPAoverall(stu);
}

static int Student_Compare_By_Property(Student *stu1, Student *stu2, const char *property) {
    static const char *dict[] = {"", "name", "id", "GPA_basic", "GPA_overall", "rank"};
    typedef enum { NONE, NAME, ID, GPA_BASIC, GPA_OVERALL, RANK } prop_type;
    int tmp = getNounIndex(dict, sizeof(dict) / sizeof(const char *), property);
    prop_type prIndex;
    if (tmp == 0)
        return -999;
    else
        prIndex = (prop_type)tmp;
    switch (prIndex) {
    case NAME:
        return strcmp(stu1->name, stu2->name);
    case ID:
        return stu1->id - stu2->id;
    case GPA_BASIC:
        return stu1->GPA_basic - stu2->GPA_basic;
    case GPA_OVERALL:
        return stu1->GPA_overall - stu2->GPA_overall;
    case RANK:
        return stu1->rank.rk - stu2->rank.rk;
    }
}

static size_t Extract_Student(Student ***_dest, int college_num, int grade,
                              const char *professionName) {
    printf("college_num: %d, grade: %d, professionName: %s\n", college_num, grade, professionName);
    int pro_num = getNounIndex(Professions[college_num], 15, professionName);
    Student_List *stu_list = Get_StudentList_by_grade(college_num * 100 + grade);
    if (stu_list == NULL)
        return 0;
    size_t arrCapacity = 16, arrSize = 0;
    Student **pstuArr = (Student **)MALLOC(sizeof(Student *) * arrCapacity);
    Student_IdNode *idNode = stu_list->first;
    // Get Students of professionName
    for (size_t i = 0; idNode != NULL; i++, idNode = idNode->next) {
        Student *crt_stu = Get_Student_by_id(idNode->id);
        if (crt_stu->major % 100 == pro_num) {
            if (i == arrCapacity) {
                Student **newArr = (Student **)MALLOC(sizeof(Student *) * (arrCapacity *= 2));
                memcpy(newArr, pstuArr, (arrCapacity / 2) * sizeof(Student *));
                FREE(pstuArr);
                pstuArr = newArr;
            }
            pstuArr[i] = crt_stu;
            ++arrSize;
        }
    }
    if (arrSize == 0)
        return 0;
    if (arrSize < arrCapacity) {
        Student **newArr = (Student **)MALLOC(sizeof(Student *) * arrSize);
        memcpy(newArr, pstuArr, sizeof(Student *) * arrSize);
        FREE(pstuArr);
        pstuArr = newArr;
    }
    *_dest = pstuArr;
    return arrSize;
}

// 更新某学院某专业某年级学生的排名
COMPARE_EXPAND(GPA_overall)
void UpdateRank(const int college_num, const int grade, const char *professionName) {
    Student **sortArr = NULL;
    size_t numOfElem = Extract_Student(&sortArr, college_num, grade, professionName);
    if (numOfElem == 0)
        return;
    qsort(sortArr, numOfElem, sizeof(Student *),
          (int (*)(const void *, const void *))Student_Compare(GPA_overall));
    for (int i = 0; i < numOfElem; i++) {
        sortArr[i]->rank.rk = i + 1;
        sortArr[i]->rank.basis = numOfElem;
    }
}

// 更新课程的优秀率、合格率
void Course_Update(const char *course_id) {
    Course *crt_Crs = Get_Course(course_id);
    Student_IdNode *idNode = crt_Crs->followed->first;
    int excellent = 0;
    int passed = 0;
    while (idNode) {
        double crs_gpa = Enroll_Find(Get_Student_by_id(idNode->id), course_id)->gpa;
        if (crs_gpa == 4.0) {
            ++excellent;
            ++passed;
        } else if (crs_gpa >= 1.0)
            ++passed;
        idNode = idNode->next;
    }
    crt_Crs->ExcellentRate = excellent / crt_Crs->followed->student_count;
    crt_Crs->PassRate = passed / crt_Crs->followed->student_count;
}

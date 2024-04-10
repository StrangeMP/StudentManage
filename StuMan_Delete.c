#include "StuMan_Delete.h"
#include "StuMan_Benefit.h"
#include "StuMan_Memory.h"
#include "StuMan_Node.h"
#include "StuMan_Search.h"
#include "StuMan_Student.h"
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define CLEAR_BEN(p)                                                                               \
    {                                                                                              \
        if (p != NULL) {                                                                           \
            while (p->next != NULL) {                                                              \
                p = p->next;                                                                       \
                FREE(p->prev);                                                                     \
            }                                                                                      \
            FREE(p);                                                                               \
        }                                                                                          \
    }

#define DELETE(p, head)                                                                            \
    {                                                                                              \
        if (p->prev != NULL) {                                                                     \
            if (p->next != NULL)                                                                   \
                p->next->prev = p->prev;                                                           \
            p->prev->next = p->next;                                                               \
        } else {                                                                                   \
            head = p->next;                                                                        \
            if (p->next != NULL)                                                                   \
                p->next->prev = NULL;                                                              \
        }                                                                                          \
        FREE(p);                                                                                   \
    }

// Student_List*,id :删除List中的指定id
int del_stulist(Student_List *pl, int aim_id) {
    Student_IdNode *p = pl->first;
    if (p == NULL)
        return -1;
    return Student_List_Erase(pl, aim_id) ? 0 : 1;
}

// 删除一个学生
typedef struct {
    int flagenroll;
    int flagname;
    int flaggrade;
    int flag;
    int pd;
} flag;

flag del_Stu(int id) {
    Student *p = Get_Student_by_id(id);
    flag f = {0, 0, 0, 0, 0};
    if (p == NULL) {
        f.flag = 1;
        return f;
    }
    // 删除课程中的学生,并删除学生的enroll
    Enroll *pe = p->enrolled;
    if (pe == NULL)
        f.flagenroll = 1; // 该学生没选课
    else
        while (pe) {
            Student_List *p3 = Get_StudentList_by_CourseID(pe->course_id);
            if (p3->first == NULL)
                printf("课程%s没有学生选择\n", pe->course_id);
            else if (del_stulist(p3, id) == 1) // 删除课程中的学生;
                printf("在课程%s中未找到学号为%d的学生\n", pe->course_id, id);
            Enroll *p4 = pe;
            pe = pe->next;
            FREE(p4); // 防止pe->next为null时无法释放
        }
    // 删除学生的benefit
    CLEAR_BEN(p->Benefits.awards);
    p->Benefits.awards = NULL;
    CLEAR_BEN(p->Benefits.projects);
    p->Benefits.projects = NULL;
    CLEAR_BEN(p->Benefits.essays);
    p->Benefits.essays = NULL;
    // 删除pd_list里面的学生
    f.pd = del_stulist(Benefits_PendingVerified, id);
    // 删除nameindex里面的学生
    Student_List *pn = Get_StudentList_by_name(p->name);
    f.flagname = del_stulist(pn, id);
    // 删除gradeindex学生
    f.flaggrade = del_stulist(Get_StudentList_by_grade(p->institute_grade), id);
    // 删除学生本身
    Student_Node *stu = Get_Student_Node_by_id(id);
    DELETE(stu, data_address.pStudentHead);
    --data_address.student_total;
    return f;
}

int del_stu_in_course(char *course_id) {
    Course *p = Get_Course(course_id);
    if (p == NULL)
        return 1; // 没有该课
    Student_IdNode *ID = p->followed->first;
    if (ID == NULL)
        return 2; // 无人选课
    while (ID) {
        Student_IdNode *next = ID->next;
        del(4, 2, ID->id, course_id);
        ID = next;
    }
    return 0;
}

int del_course(char *course_id) {
    // 删除选课学生
    int f = del_stu_in_course(course_id);
    if (f == 1)
        return 1;
    Course_Node *pc = Get_Course_Node(course_id);
    DELETE(pc, data_address.pCourseHead);
    --data_address.course_total;
    return f;
}

// 1,1,int    id: del stu
// 2,1,course_id: del course
// 3,1,course_id: del all stu in course
// 4,2,stu_id,crs_id: del crs of a stu
void del(int which, int num, ...) {
    va_list valist;
    va_start(valist, num);
    switch (which) {
    case 1: // 删除学生
    {
        int aimId = va_arg(valist, int);
        flag flag = del_Stu(aimId);
        int erro = 0;
        if (flag.flag == 1) {
            printf("未找到该学生\n");
            return;
        } else {
            if (flag.flagenroll == 1) {
                printf("学号为%d的学生没有选课\n", aimId);
                erro = 1;
            }
            if (flag.flaggrade == 1) {
                printf("在年级索引中未找到学号为%d的学生\n", aimId);
                erro = 1;
            }
            if (flag.flagname == 1) {
                printf("在姓名索引中未找到学号为%d的学生\n", aimId);
                erro = 1;
            }
        }
        if (erro == 0)
            printf("已删除学号为%d的学生\n", aimId);
        break;
    }

    case 2: // 删除课程
    {
        char *course_id = va_arg(valist, char *);
        char id[13];
        strcpy(id, course_id);
        int f = del_course(course_id);
        if (f == 1)
            printf("未找到课程号为%s的课程\n", id);
        else if (f == 2)
            printf("该课程没有任何学生选择\n");
        else if (f == 0) {
            printf("已删除课程号为%s的课程\n", id);
        }
        break;
    }

    case 3: // 删除课程中所有学生
    {
        char *course_id = va_arg(valist, char *);
        char id[13];
        strcpy(id, course_id);
        int f = del_stu_in_course(course_id);
        if (f == 0)
            printf("已清空课程号为%s的课程中的学生\n", id);
        else if (f == 1)
            printf("未找到课程号为%s的课程\n", id);
        else if (f == 2)
            printf("该课程没有任何学生选择\n");
        break;
    }

    case 4: // 删除某学生的某课程
    {
        int stu_id = va_arg(valist, int);
        char *crs_id = va_arg(valist, char *);
        char cr1[13];
        char crs[13];
        Student *p = Get_Student_by_id(stu_id);
        Enroll *e = p->enrolled;
        strcpy(crs, crs_id);
        if (p == NULL)
            printf("未找到学号为%d的学生\n", stu_id);
        while (e) {
            strcpy(cr1, e->course_id);
            if (strcmp(cr1, crs_id) == 0) {
                Student_List *pl = Get_StudentList_by_CourseID(e->course_id);
                if (pl->first == NULL)
                    printf("课程%s没有学生选择\n", e->course_id);
                else if (del_stulist(pl, stu_id) == 1)
                    printf("在课程%s中未找到学号为%d的学生\n", e->course_id, stu_id);
                // 删除课程中的学生
                DELETE(e, p->enrolled);
                printf("已删除学号为%d的学生课程号为%s的课程\n", stu_id, crs);
                va_end(valist);
                return;
            }
            e = e->next;
        }
        printf("该学生所选课程中未找到课程号为%s的课程", crs);
        break;
    }
    }
    va_end(valist);
    return;
}

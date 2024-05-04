#include "StuMan_Memory.h"
#include "StuMan_Account.h"
#include "StuMan_Student.h"
#include <stdlib.h>
#ifdef DEBUG
MemoryRecord memRec = {0, {NULL}};
#endif

void *MALLOC(size_t size) {
#ifdef DEBUG
    memRec.table[memRec.cnt] = malloc(size);
    return memRec.table[memRec.cnt++];
#else
    return malloc(size);
#endif
}

void FREE(void *ptr) {
    free(ptr);
#ifdef DEBUG
    for (int i = 0; i < memRec.cnt; i++) {
        if (memRec.table[i] == ptr) {
            memRec.table[i] = NULL;
            break;
        }
    }
#endif
}

static void Student_Node_Free(Student_Node *res) {
    Student_Node *last = NULL, *crt = res;
    while (crt) {
        last = crt;

        Enroll *lastEnroll = NULL, *crtEnroll = crt->stu.enrolled;
        while (crtEnroll) {
            lastEnroll = crtEnroll;
            crtEnroll = crtEnroll->next;
            FREE(lastEnroll);
        }
        Essay *lastEssay = NULL, *crtEssay = crt->stu.Benefits.essays;
        while (crtEssay) {
            lastEssay = crtEssay;
            crtEssay = crtEssay->next;
            FREE(lastEssay);
        }
        Project *lastProject = NULL, *crtProject = crt->stu.Benefits.projects;
        while (crtProject) {
            lastProject = crtProject;
            crtProject = crtProject->next;
            FREE(lastProject);
        }
        Award *lastAward = NULL, *crtAward = crt->stu.Benefits.awards;
        while (crtAward) {
            lastAward = crtAward;
            crtAward = crtAward->next;
            FREE(lastAward);
        }

        crt = crt->next;
        FREE(last);
    }
}

static void Student_List_Free(Student_List *stu_list) {
    if (stu_list == NULL || stu_list->first == NULL)
        return;
    Student_IdNode *last_node = NULL;
    Student_IdNode *crt_node = stu_list->first;
    while (crt_node) {
        last_node = crt_node;
        crt_node = crt_node->next;
        FREE(last_node);
    }
    FREE(stu_list);
}

static void Course_Node_Free(Course_Node *res) {
    Course_Node *last = NULL, *crt = res;
    while (crt) {
        last = crt;
        Student_List_Free(crt->crs.followed);
        crt = crt->next;
        FREE(last);
    }
}

void ReleaseResource() {
    Student_Node_Free(data_address.pStudentHead);
    Course_Node_Free(data_address.pCourseHead);

    for (int i = 0; i < 65536; i++)
        Student_List_Free(nameIndex[i]);
    for (int i = 0; i < 90; i++)
        for (int j = 0; j < 4; j++)
            Student_List_Free(gradeIndex[i][j]);
    Student_List_Free(Benefits_PendingVerified);
    CleanTeachers();
}
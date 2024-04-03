#include "StuMan_Benefit.h"
#include "StuMan_Delete.h"
#include "StuMan_Import.h"
#include "StuMan_Memory.h"
#include "StuMan_Node.h"
#include "StuMan_Search.h"
#include "StuMan_Student.h"
#include <stdio.h>
#include <stdlib.h>

Student_List *Benefits_PendingVerified = NULL;

static bool Check_Pending(Student *stu) {
    Essay *e = stu->Benefits.essays;
    while (e) {
        if (e->status == PENDING)
            return true;
        e = e->next;
    }
    Award *a = stu->Benefits.awards;
    while (a) {
        if (a->status == PENDING)
            return true;
        a = a->next;
    }
    Project *p = stu->Benefits.projects;
    while (p) {
        if (p->status == PENDING)
            return true;
        p = p->next;
    }
    return false;
}

static void pendingList_Update(Student *stu) {
    if (Benefits_PendingVerified == NULL) {
        Benefits_PendingVerified = (Student_List *)MALLOC(sizeof(Student_List));
        Benefits_PendingVerified->first = Benefits_PendingVerified->end = NULL;
        Benefits_PendingVerified->student_count = 0;
    }
    Student_IdNode *node = Benefits_PendingVerified->first;
    while (node) {
        if (node->id == stu->id)
            break;
        node = node->next;
    }
    if (node) {
        if (Check_Pending(stu))
            return;
        else {
            if (node == Benefits_PendingVerified->first) {
                Benefits_PendingVerified->first = node->next;
                if (Benefits_PendingVerified->first)
                    Benefits_PendingVerified->first->prev = NULL;
            } else {
                if (node->next) {
                    node->next->prev = node->prev;
                }
                if (node->prev) {
                    node->prev->next = node->next;
                }
            }
            FREE(node);
            --(Benefits_PendingVerified->student_count);
        }
    } else {
        if (Check_Pending(stu))
            Student_List_AddStudentID(Benefits_PendingVerified, stu->id);
    }
}

Essay *Essay_Find(Essay *list, Essay *key) {
    Essay *node = list;
    while (node) {
        if (strcmp(node->Title, key->Title) == 0)
            return node;
        node = node->next;
    }
    return node;
}

Project *Project_Find(Project *list, Project *key) {
    Project *node = list;
    while (node) {
        if (strcmp(node->ProjectID, key->ProjectID) == 0)
            return node;
        node = node->next;
    }
    return node;
}

Award *Award_Find(Award *list, Award *key) {
    Award *node = list;
    while (node) {
        if (strcmp(node->CompetitionName, key->CompetitionName) == 0)
            return node;
        node = node->next;
    }
    return node;
}

void Student_AddEssay(int stu_id, Essay *essay) {
    Student *student = Get_Student_by_id(stu_id);
    if (student->Benefits.essays == NULL) {
        essay->prev = NULL;
        essay->next = NULL;
        student->Benefits.essays = essay;
        student->Benefits.essays->prev = NULL;
        student->Benefits.essays->next = NULL;
    } else {
        Essay *e = Essay_Find(student->Benefits.essays, essay);
        if (e) {
            e->status = essay->status;
            FREE(essay);
        } else {
            essay->next = student->Benefits.essays;
            essay->prev = NULL;
            essay->next->prev = essay;
            student->Benefits.essays = essay;
        }
    }
    pendingList_Update(student);
}

void Student_AddAward(int stu_id, Award *award) {
    Student *student = Get_Student_by_id(stu_id);
    if (student->Benefits.awards == NULL) {
        award->prev = NULL;
        award->next = NULL;
        student->Benefits.awards = award;
        student->Benefits.awards->prev = NULL;
        student->Benefits.awards->next = NULL;
    } else {
        Award *a = Award_Find(student->Benefits.awards, award);
        if (a) {
            a->status = award->status;
            FREE(award);
        } else {
            award->next = student->Benefits.awards;
            award->prev = NULL;
            award->next->prev = award;
            student->Benefits.awards = award;
        }
    }
    pendingList_Update(student);
}

void Student_AddProject(int stu_id, Project *project) {
    Student *student = Get_Student_by_id(stu_id);
    if (student->Benefits.projects == NULL) {
        project->prev = NULL;
        project->next = NULL;
        student->Benefits.projects = project;
        student->Benefits.projects->prev = NULL;
        student->Benefits.projects->next = NULL;
    } else {
        Project *e = Project_Find(student->Benefits.projects, project);
        if (e) {
            e->status = project->status;
            FREE(project);
        } else {
            project->next = student->Benefits.projects;
            project->prev = NULL;
            project->next->prev = project;
            student->Benefits.projects = project;
        }
    }
    pendingList_Update(student);
}

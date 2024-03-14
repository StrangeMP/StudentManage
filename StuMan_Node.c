#include "StuMan_Node.h"
#include <stdlib.h>

// Searches a given id in a Student_IdNode Linked-List starting at Head.
// If found, returns the node. Otherwise returns NULL.
Student_IdNode *Student_IdNode_Find(Student_IdNode *Head, int id) {
    if (Head == NULL)
        return NULL;
    Student_IdNode *idNode = Head;
    while (true) {
        if (idNode->id == id)
            return idNode;
        else if (idNode->next == NULL)
            return NULL;
        else
            idNode = idNode->next;
    }
}

// Insert item before target, returns item;
static Student_IdNode *Student_IdNode_InsertBefore(Student_IdNode *target, int stu_id) {
    if (target == NULL)
        return NULL;
    Student_IdNode *item = (Student_IdNode *)malloc(sizeof(Student_IdNode));
    item->id = stu_id;
    Student_IdNode *first = target->prev, *second = item, *third = target;
    second->next = third;
    third->prev = second;
    if (first == NULL) {
        second->prev = NULL;
        return second;
    } else {
        second->prev = first;
        first->next = second;
        return second;
    }
}

// Insert item after target, returns item;
static Student_IdNode *Student_IdNode_InsertAfter(Student_IdNode *target, int stu_id) {
    if (target == NULL)
        return NULL;
    Student_IdNode *item = (Student_IdNode *)malloc(sizeof(Student_IdNode));
    item->id = stu_id;
    Student_IdNode *first = target, *second = item, *third = target->next;
    first->next = second;
    second->prev = first;
    if (third == NULL) {
        second->next = NULL;
        return second;
    } else {
        second->next = third;
        third->prev = second;
        return second;
    }
}

// Adds a given id right behind Head if the id doesn't exist, returns the added node.
// If adding failed or the id already exists, return NULL.
// If Head == NULL, constructs a new Student_IdNode with the given id, returns the node.
Student_IdNode *Student_IdNode_Add(Student_IdNode *Head, int id) {
    Student_IdNode *idNode = NULL;
    if (Head != NULL) {
        idNode = Student_IdNode_Find(Head, id);
        if (idNode != NULL)
            // id already exists
            return NULL;
        else
            // id doesn't exist, add
            Student_IdNode_InsertAfter(Head, id);
    }
    // Head == NULL
    else { // Construct a new Student_IdNode with the given id
        idNode = (Student_IdNode *)malloc(sizeof(Student_IdNode));
        idNode->id = id;
        idNode->prev = NULL;
        idNode->next = NULL;
    }
    return idNode;
}

// Add a Student_IdNode to stu_list, if stu_list empty, initialize it with id.
// Returns stu_list if added or initialized, NULL if id already exists.
Student_List *Student_List_AddStudentID(Student_List *stu_list, int id) {
    if (stu_list == NULL) {
        stu_list = (Student_List *)malloc(sizeof(Student_List));
        stu_list->end = stu_list->first = Student_IdNode_Add(NULL, id);
        stu_list->student_count = 1;
        return stu_list;
    } else if (stu_list->student_count == 0 || stu_list->first == NULL) {
        stu_list->end = stu_list->first = Student_IdNode_Add(NULL, id);
        stu_list->student_count = 1;
        return stu_list;
    } else if (Student_IdNode_Add(stu_list->first, id) != NULL) {
        stu_list->student_count++;
        return stu_list;
    } else
        return NULL;
}
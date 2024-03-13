#include "Student.h"
#include "Instit.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YEAR 24

static Student *idIndex[90][4][30][30] = {NULL};           // 学号-学生内存地址索引
static Student_IdNode *nameIndex[65536] = {NULL};          // 名字-学号索引
static Student_List gradeIndex[90][4] = {{0, NULL, NULL}}; // 学院年级-学号索引

// id =212305
static void Build_ID_Index(Student *pStu) {
    int id = pStu->id;
    idIndex[id / 1000000][id / 10000 % 100][id / 100 % 100][id % 100] = pStu;
}

static size_t Get_16bit_Hash(const char str[]) {
    int hash = 0;
    int len = 0;
    for (; str[len] != L'\0'; len++)
        ;
    const char *ch = str;
    for (int i = 0; i < len; i++) {
        hash = hash + ((hash) << 5) + *(ch + i) + ((*(ch + i)) << 7);
    }
    return ((hash) ^ (hash >> 16)) & 0xffff;
}

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

// Adds a given id right behind Head if the id doesn't exist, returns the added node.
// If adding failed or the id already exists, return NULL.
// If Head == NULL, constructs a new Student_IdNode with the given id, returns the node.
Student_IdNode *Student_IdNode_Add(Student_IdNode *Head, int id) {
    Student_IdNode *idNode = NULL;
    if (Head != NULL) {
        idNode = Student_IdNode_Find(Head, id);
        if (idNode != NULL)
            return NULL;
        else {
            idNode = (Student_IdNode *)malloc(sizeof(Student_IdNode));
            if (idNode == NULL)
                return NULL;
            idNode->id = id;
            idNode->next = Head->next;
            Head->next = idNode;
        }

    }
    // Head == NULL
    else {
        idNode = (Student_IdNode *)malloc(sizeof(Student_IdNode));
        idNode->id = id;
        idNode->next = NULL;
    }
    return idNode;
}

static void Build_Name_Index(Student *pStu) {
    // try fetch the name from index
    Student_IdNode *idNode = nameIndex[Get_16bit_Hash(pStu->name)];
    // check if the name already exists
    if (idNode != NULL) {
        // name exists, try appending
        if (Student_IdNode_Add(idNode, pStu->id) == NULL)
            printf("Build_Name_Index Error: %s exists but appending %d failed\n", pStu->name,
                   pStu->id);
    } else {
        // name doesn't exist, initialize an idNode under the name
        nameIndex[Get_16bit_Hash(pStu->name)] = Student_IdNode_Add(NULL, pStu->id);
    }
}

static void Build_Grade_Index(Student *pStu) {
    int institute = pStu->institute_grade / 100;
    int grade = YEAR - pStu->institute_grade % 100 - 1;
    Student_List *pList = &gradeIndex[institute][grade];
    Student_IdNode *node = pList->first;
    if (Student_IdNode_Add(node, pStu->id) != NULL) {
        pList->end = node;
        pList->student_count++;
    }
}

void Build_Student_Index(Student *pStu) {
    if (Get_Student_by_id(pStu->id) == NULL) {
        Build_ID_Index(pStu);
        Build_Name_Index(pStu);
        Build_Grade_Index(pStu);
    }
}

Student *Get_Student_by_id(const int id) {
    return idIndex[id / 1000000][id / 10000 % 100][id / 100 % 100][id % 100];
}

Student_IdNode *Get_Students_by_name(const char name[]) {
    Student_IdNode *id_List = nameIndex[Get_16bit_Hash(name)];
    return id_List;
}

const Student_List Get_StudentList_by_grade(int institute_and_grade) {
    return gradeIndex[institute_and_grade / 100][institute_and_grade % 100];
}

Course *Get_Course(const char *course_id) {
    Course_Node *crs_node = data_address.pCourseHead;
    if (crs_node == NULL)
        return NULL;
    while (true) {
        if (strcmp(crs_node->crs.id, course_id) == 0)
            return &crs_node->crs;
        else if (crs_node->next == NULL)
            break;
        else
            crs_node = crs_node->next;
    }
    // not found
    return NULL;
}

Enroll *Enroll_Construct(const cJSON *item) {
    if (item == NULL)
        return NULL;
    Enroll *newEnr = (Enroll *)malloc(sizeof(Enroll));
    strcpy(newEnr->course_id, cJSON_GetObjectItem(item, "course_id")->valuestring);
    cJSON *cjson_test = cJSON_GetObjectItem(item, "grade");
    if (cjson_test == NULL)
        newEnr->level = cJSON_GetObjectItem(item, "level")->valueint;
    else
        newEnr->grade = cjson_test->valuedouble;
    newEnr->retake = cJSON_GetObjectItem(item, "retake")->valueint;
    newEnr->major = cJSON_GetObjectItem(item, "major")->valueint;
    newEnr->gpa = cJSON_GetObjectItem(item, "gpa")->valuedouble;
    newEnr->passed = cJSON_GetObjectItem(item, "passed")->valueint;
    newEnr->effective = cJSON_GetObjectItem(item, "effective")->valueint;
    newEnr->next = NULL;
    return newEnr;
}

Enroll *Enroll_Find(Student *src, const char *course_id) {
    if (src->enrolled == NULL)
        return NULL;
    Enroll *enroll_node = src->enrolled;
    while (true) {
        if (strcmp(enroll_node->course_id, course_id) == 0)
            return enroll_node;
        else if (enroll_node->next == NULL)
            return NULL;
        else
            enroll_node = enroll_node->next;
    }
}

// Adds cjson_enrolled to dest if possible, returns the added node if added, otherwise returns NULL.
// Course follower-list is checked and updated if possible, regardless of
// whether dest has enrolled in the course;
Enroll *Enroll_Append(Student *dest, cJSON *cjson_enrolled) {
    if (dest == NULL || cjson_enrolled == NULL)
        return NULL;
    Enroll *ret = NULL;
    char course_id[13];
    strcpy(course_id, cJSON_GetObjectItem(cjson_enrolled, "course_id")->valuestring);
    Enroll *item = Enroll_Find(dest, course_id);
    if (item == NULL) { // course_id not found in enrolled-list
        ret = item = Enroll_Construct(cjson_enrolled);
        if (dest->enrolled == NULL)
            dest->enrolled = item;
        else {
            item->next = dest->enrolled;
            dest->enrolled = item;
        }
    }
    // Try updating Course follower-list
    // search for the course_id in the Course List
    // if found, add his student_id to the course
    // if not found, print an error message but not terminate
    Course *crs_dest = Get_Course(item->course_id); //
    if (crs_dest) {
        // course_id found in Course List
        Student_IdNode *idnode = crs_dest->followed;
        if (idnode == NULL) { // crs_dest followed-list empty, initialize a new node.
            crs_dest->followed = Student_IdNode_Add(NULL, dest->id);
        } else
            // Try add dest->id to crs_dest->followed
            // crs_dest->followed is guaranteed not to be empty at this point
            Student_IdNode_Add(crs_dest->followed, dest->id);
    } else
        // course_id not found in Course List, no such Course, error.
        printf("Enroll_Append Error: Course %s not found when appending for %s:%d", item->course_id,
               dest->name, dest->id);
    return ret;
}

// Insert a new Student_Node to database.
// If the student already exists, update enrolled_List(if any).
// Returns a pointer to that student, regardless of inserted or not.
Student *Student_Insert(const cJSON *cjson_student) {
    // get Student id
    int id = cJSON_GetObjectItem(cjson_student, "id")->valueint;
    Student *currStudent = Get_Student_by_id(id); // current Student
    // check if the id exists, if the id doesn't exist yet
    // Construct a new Student and insert to database
    if (currStudent == NULL) { // id doesn't exist
        Student_Node *currStudentNode = (Student_Node *)malloc(sizeof(Student_Node));
        currStudentNode->next = NULL;
        data_address.pStudentFoot->next = currStudentNode;
        data_address.pStudentFoot = data_address.pStudentFoot->next;
        // initialize the new Node of Student
        currStudent = &currStudentNode->stu;
        strcpy(currStudent->name, cJSON_GetObjectItem(cjson_student, "name")->valuestring);
        currStudent->id = id;
        currStudent->institute_grade =
            cJSON_GetObjectItem(cjson_student, "institute_grade")->valueint;
        currStudent->enrolled = NULL;
        Build_Student_Index(currStudent);
    }

    // Process with enrolled courses
    cJSON *Enrolled_Collection = cJSON_GetObjectItem(cjson_student, "enrolled");
    int course_array_size = cJSON_GetArraySize(Enrolled_Collection);
    for (int i = 0; i < course_array_size; i++) {
        cJSON *cjson_enrolled = cJSON_GetArrayItem(Enrolled_Collection, i);
        Enroll_Append(currStudent, cjson_enrolled);
    }

    return currStudent;
}

// Insert a new Course to database. Already existing Course won't be inserted.
// Check and update the follower-list. Only those who have enrolled in the Course
// would be added to the follower-list.
Course *Course_Insert(const cJSON *cjson_course) {
    // get Course id
    char course_id[13];
    strcpy(course_id, cJSON_GetObjectItem(cjson_course, "id")->valuestring);
    Course *currCourse = Get_Course(course_id); // current Course
    // check if the id exists, if the id doesn't exist yet
    // Construct a new Course and insert to database
    if (currCourse == NULL) { // course doesn't exist in database, new Course
        Course_Node *currCourseNode = (Course_Node *)malloc(sizeof(Course_Node));
        currCourseNode->next = NULL;
        data_address.pCourseFoot->next = currCourseNode;
        data_address.pCourseFoot = data_address.pCourseFoot->next;
        // initialize the new Node of Course
        currCourse = &currCourseNode->crs;
        strcpy(currCourse->name, cJSON_GetObjectItem(cjson_course, "name")->valuestring);
        strcpy(currCourse->id, cJSON_GetObjectItem(cjson_course, "id")->valuestring);
        strcpy(currCourse->teacher, cJSON_GetObjectItem(cjson_course, "teacher")->valuestring);
        strcpy(currCourse->exam_date, cJSON_GetObjectItem(cjson_course, "exam_date")->valuestring);
        currCourse->semester = cJSON_GetObjectItem(cjson_course, "semester")->valueint;
        currCourse->pub_category = cJSON_GetObjectItem(cjson_course, "pub_category")->valueint;
        currCourse->type = cJSON_GetObjectItem(cjson_course, "type")->valueint;
        currCourse->nature = cJSON_GetObjectItem(cjson_course, "nature")->valueint;
        currCourse->credit = cJSON_GetObjectItem(cjson_course, "credit")->valueint;
        currCourse->hours = cJSON_GetObjectItem(cjson_course, "hours")->valueint;
        currCourse->exam_type = cJSON_GetObjectItem(cjson_course, "exam_type")->valueint;
        currCourse->grade_type = cJSON_GetObjectItem(cjson_course, "grade_type")->valueint;
        currCourse->institute = cJSON_GetObjectItem(cjson_course, "institute")->valueint;
        currCourse->followed = NULL;
    }
    // Process with follower-list
    cJSON *followed_List = cJSON_GetObjectItem(cjson_course, "followed");
    int followed_array_size = cJSON_GetArraySize(followed_List);
    for (int i = 0; i < followed_array_size; i++) {
        int crt_follower_id = cJSON_GetArrayItem(followed_List, i)->valueint;
        Student *follower = Get_Student_by_id(crt_follower_id);
        if (follower == NULL)
            printf("Course_Insert Error: Course %s:%s follower_id %d not found in database.\n",
                   currCourse->name, currCourse->id, crt_follower_id);
        else {
            Enroll *enroll_node = follower->enrolled;
            if (enroll_node == NULL || Enroll_Find(follower, currCourse->id) == NULL)
                return currCourse;
            else
                Student_IdNode_Add(currCourse->followed, crt_follower_id);
        }
    }
}

void ImportData(const char *fileDir) {
    if (fileDir == NULL) {
        printf("ImportData Error: invalid fileDir\n");
        return;
    }
    FILE *pf = fopen(fileDir, "r");
    fseek(pf, 0, SEEK_END);
    long fsize = ftell(pf);
    fseek(pf, 0, SEEK_SET);
    char *rawData = (char *)malloc(fsize + 1);
    fread(rawData, fsize, 1, pf);
    rawData[fsize] = '\0';
    fclose(pf);

    cJSON *cjson_Data = cJSON_Parse(rawData);
    // Process with to-be-imported Students
    cJSON *Student_Collection = cJSON_GetObjectItem(cjson_Data, "Students");
    int student_array_size = cJSON_GetArraySize(Student_Collection);
    for (int i = 0; i < student_array_size; i++) {
        cJSON *cjson_student = cJSON_GetArrayItem(Student_Collection, i);
        Student_Insert(cjson_student);
    }

    // Process with to-be-imported Courses
    cJSON *Course_Collection = cJSON_GetObjectItem(cjson_Data, "Courses");
    int course_array_size = cJSON_GetArraySize(Course_Collection);
    for (int i = 0; i < course_array_size; i++) {
        cJSON *cjson_course = cJSON_GetArrayItem(Course_Collection, i);
        Course_Insert(cjson_course);
    }
}
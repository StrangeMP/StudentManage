#include "StuMan_Import.h"
#include "StuMan_BuildIndex.h"
#include "StuMan_Node.h"
#include "StuMan_Nouns.h"
#include "StuMan_Search.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Enroll *Enroll_Construct(const cJSON *item);
static Enroll *Enroll_Append(Student *dest, cJSON *cjson_enrolled);
static Student *Student_Insert(const cJSON *cjson_student);
static Course *Course_Insert(const cJSON *cjson_course);
void ImportData(const char *fileDir);

static int getNounIndex(const char **nounArr, int arrLen, const char *content) {
    if (content == NULL)
        return 0;
    for (int i = 1; i < arrLen; i++) {
        if (strcmp(nounArr[i], content) == 0)
            return i;
    }
    return 0;
}

static int parse_Institute_grade(const cJSON *item) {
    int instit = getNounIndex(Institutes, Get_NounArrLen(Institutes),
                              cJSON_GetObjectItem(item, "学院")->valuestring);
    int grade = cJSON_GetObjectItem(item, "年级")->valueint;
    return instit * 100 + grade % 100;
}

static Enroll *Enroll_Construct(const cJSON *item) {
    if (item == NULL)
        return NULL;
    Enroll *newEnr = (Enroll *)malloc(sizeof(Enroll));
    strcpy(newEnr->course_id, cJSON_GetObjectItem(item, "课程号")->valuestring);
    if (cJSON_IsNumber(cJSON_GetObjectItem(item, "总成绩")))
        newEnr->grade = cJSON_GetObjectItem(item, "总成绩")->valuedouble;
    else
        newEnr->level = getNounIndex(Enroll_level, Get_NounArrLen(Enroll_level),
                                     cJSON_GetObjectItem(item, "总成绩")->valuestring);
    int tmp1, tmp2;
    sscanf(cJSON_GetObjectItem(item, "学年学期")->valuestring, "%*c%*c%d-%*d-%d", &tmp1, &tmp2);
    newEnr->semester = tmp1 * 100 + tmp2;
    newEnr->retake = getNounIndex(Enroll_retake, Get_NounArrLen(Enroll_retake),
                                  cJSON_GetObjectItem(item, "重修重考")->valuestring);
    newEnr->major = strcmp(cJSON_GetObjectItem(item, "是否主修")->valuestring, "否");
    newEnr->gpa = cJSON_GetObjectItem(item, "绩点")->valuedouble;
    newEnr->passed = strcmp(cJSON_GetObjectItem(item, "是否及格")->valuestring, "否");
    newEnr->effective = strcmp(cJSON_GetObjectItem(item, "是否有效")->valuestring, "否");
    newEnr->prev = NULL;
    newEnr->next = NULL;
    return newEnr;
}

// Adds cjson_enrolled to dest if possible, returns the added node if added, otherwise returns NULL.
// Course follower-list is checked and updated if possible, regardless of whether dest has enrolled
// in the course;
static Enroll *Enroll_Append(Student *dest, cJSON *cjson_enrolled) {
    if (dest == NULL || cjson_enrolled == NULL)
        return NULL;
    Enroll *ret = NULL;
    char course_id[13];
    strcpy(course_id, cJSON_GetObjectItem(cjson_enrolled, "课程号")->valuestring);
    Enroll *item = Enroll_Find(dest, course_id);
    if (item == NULL) { // course_id not found in enrolled-list
        ret = item = Enroll_Construct(cjson_enrolled);
        if (dest->enrolled == NULL) // Student dest has no enrolled courses
            dest->enrolled = item;
        else {
            // Student dest has already enrolled in course(s)
            // Add item as the first node of enrolled-list
            dest->enrolled->prev = item;
            item->next = dest->enrolled;
            item->prev = NULL;
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
        Student_List *stu_list = crs_dest->followed;
        if (stu_list == NULL) // crs_dest followed-list empty, initialize a new node.
            stu_list = crs_dest->followed = Student_List_AddStudentID(NULL, dest->id);
        else
            Student_List_AddStudentID(stu_list, dest->id);

    } else // course_id not found in Course List, no such Course, error.
        printf("Enroll_Append Error: Course %s not found when appending for %s:%d", item->course_id,
               dest->name, dest->id);
    return ret;
}

// Add a Student_Node based on id right behind Head if item doesn't exist,
// return the added node. If adding failed or corresponding node already exists, return NULL.
// If Head==NULL, return a Student_Node newly constructed with id.
static Student_Node *Student_Node_Add(Student_Node *Head, int id) {
    if (Get_Student_by_id(id) != NULL)
        return NULL;
    else {
        Student_Node *newNode = (Student_Node *)malloc(sizeof(Student_Node));
        newNode->stu.id = id;
        if (Head == NULL) {
            newNode->prev = newNode->next = NULL;
            return newNode;
        } else {
            newNode->next = Head->next;
            if (newNode->next != NULL)
                newNode->next->prev = newNode;
            newNode->prev = Head;
            Head->next = newNode;
            return newNode;
        }
    }
}

// Insert a new Student_Node to database.
// If the student already exists, update enrolled_List(if any).
// Returns a pointer to that student, regardless of inserted or not.
static Student *Student_Insert(const cJSON *cjson_student) {
    // get Student id
    int id = cJSON_GetObjectItem(cjson_student, "学号")->valueint;
    Student *currStudent = Get_Student_by_id(id); // current Student

    if (currStudent == NULL) {
        // id doesn't exist, construct a new Student and insert to database
        if (data_address.pStudentHead == NULL) {
            data_address.pStudentFoot = data_address.pStudentHead = Student_Node_Add(NULL, id);
            currStudent = &data_address.pStudentFoot->stu;
        } else {
            data_address.pStudentFoot = Student_Node_Add(data_address.pStudentFoot, id);
            currStudent = &data_address.pStudentFoot->stu;
        }
        // initialize the new Node of Student
        strcpy(currStudent->name, cJSON_GetObjectItem(cjson_student, "姓名")->valuestring);
        currStudent->id = id;
        currStudent->institute_grade = parse_Institute_grade(cjson_student);
        currStudent->enrolled = NULL;
        Build_Student_Index(currStudent);
    }
    return currStudent;
}

// Add a Course_Node based on course_id right behind Head if item doesn't exist,
// return the added node. If adding failed or corresponding node already exists, return NULL.
// If Head==NULL, return a Course_Node newly constructed with course_id.
static Course_Node *Course_Node_Add(Course_Node *Head, const char *course_id) {
    if (Get_Course(course_id) != NULL)
        return NULL;
    else {
        Course_Node *newNode = (Course_Node *)malloc(sizeof(Course_Node));
        strcpy(newNode->crs.id, course_id);
        if (Head == NULL) {
            newNode->prev = newNode->next = NULL;
            return newNode;
        } else {
            newNode->next = Head->next;
            if (newNode->next != NULL)
                newNode->next->prev = newNode;
            newNode->prev = Head;
            Head->next = newNode;
            return newNode;
        }
    }
}

// Insert a new Course to database. Already existing Course won't be inserted.
// Check and update the follower-list. Only those who have enrolled in the Course
// would be added to the follower-list.
static Course *Course_Insert(const cJSON *cjson_course) {
    // get Course id
    char course_id[13];
    strcpy(course_id, cJSON_GetObjectItem(cjson_course, "课程号")->valuestring);
    Course *currCourse = Get_Course(course_id); // current Course
    // check if the id exists, if the id doesn't exist yet
    // Construct a new Course and insert to database
    if (currCourse == NULL) { // course doesn't exist in database, new Course
        if (data_address.pCourseHead == NULL) {
            data_address.pCourseHead = data_address.pCourseFoot = Course_Node_Add(NULL, course_id);
            currCourse = &data_address.pCourseHead->crs;
        } else {
            data_address.pCourseFoot = Course_Node_Add(data_address.pCourseFoot, course_id);
            currCourse = &data_address.pCourseFoot->crs;
        }
        strcpy(currCourse->name, cJSON_GetObjectItem(cjson_course, "课程名")->valuestring);
        strcpy(currCourse->teacher, cJSON_GetObjectItem(cjson_course, "上课老师")->valuestring);
        strcpy(currCourse->exam_date, cJSON_GetObjectItem(cjson_course, "考试日期")->valuestring);
        currCourse->pub_category =
            getNounIndex(pubCourseCategory, Get_NounArrLen(pubCourseCategory),
                         cJSON_GetObjectItem(cjson_course, "校公选课类别")->valuestring);
        currCourse->type = getNounIndex(Course_type, Get_NounArrLen(Course_type),
                                        cJSON_GetObjectItem(cjson_course, "课程类别")->valuestring);
        currCourse->nature =
            getNounIndex(Course_nature, Get_NounArrLen(Course_nature),
                         cJSON_GetObjectItem(cjson_course, "课程性质")->valuestring);
        currCourse->credit = cJSON_GetObjectItem(cjson_course, "学分")->valueint;
        currCourse->hours = cJSON_GetObjectItem(cjson_course, "学时")->valueint;
        currCourse->exam_type =
            getNounIndex(Course_exam_type, Get_NounArrLen(Course_exam_type),
                         cJSON_GetObjectItem(cjson_course, "考试类型")->valuestring);
        currCourse->grade_type =
            getNounIndex(Course_grade_type, Get_NounArrLen(Course_grade_type),
                         cJSON_GetObjectItem(cjson_course, "显示等级成绩类型")->valuestring);
        currCourse->institute =
            getNounIndex(Institutes, Get_NounArrLen(Institutes),
                         cJSON_GetObjectItem(cjson_course, "开课单位")->valuestring);
        ;
        currCourse->followed = NULL;
    }
    return currCourse;
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
    cJSON *Student_Collection = cJSON_GetObjectItem(cjson_Data, "学生");
    int student_array_size = cJSON_GetArraySize(Student_Collection);
    for (int i = 0; i < student_array_size; i++) {
        cJSON *cjson_student = cJSON_GetArrayItem(Student_Collection, i);
        Student_Insert(cjson_student);
    }

    // Process with to-be-imported Courses
    cJSON *Course_Collection = cJSON_GetObjectItem(cjson_Data, "课程");
    int course_array_size = cJSON_GetArraySize(Course_Collection);
    for (int i = 0; i < course_array_size; i++) {
        cJSON *cjson_course = cJSON_GetArrayItem(Course_Collection, i);
        Course_Insert(cjson_course);
    }

    // Deal with following
    for (int i = 0; i < student_array_size; i++) {
        cJSON *cjson_student = cJSON_GetArrayItem(Student_Collection, i);
        // Process with enrolled courses
        cJSON *Enrolled_Collection = cJSON_GetObjectItem(cjson_student, "所选课程");
        int course_array_size = cJSON_GetArraySize(Enrolled_Collection);
        Student *currStudent =
            Get_Student_by_id(cJSON_GetObjectItem(cjson_student, "学号")->valueint);
        for (int i = 0; i < course_array_size; i++) {
            cJSON *cjson_enrolled = cJSON_GetArrayItem(Enrolled_Collection, i);
            Enroll_Append(currStudent, cjson_enrolled);
        }
    }
    for (int i = 0; i < course_array_size; i++) {
        cJSON *cjson_course = cJSON_GetArrayItem(Course_Collection, i);
        // Process with follower-list
        cJSON *followed_List = cJSON_GetObjectItem(cjson_course, "选课学生学号");
        int followed_array_size = cJSON_GetArraySize(followed_List);
        Course *currCourse = Get_Course(cJSON_GetObjectItem(cjson_course, "课程号")->valuestring);
        for (int i = 0; i < followed_array_size; i++) {
            int crt_follower_id = cJSON_GetArrayItem(followed_List, i)->valueint;
            Student *follower = Get_Student_by_id(crt_follower_id);
            if (follower == NULL)
                printf("Course_Insert Error: Course %s:%s follower_id %d not found in database.\n",
                       currCourse->name, currCourse->id, crt_follower_id);
            else {
                Enroll *enroll_node = follower->enrolled;
                if (enroll_node == NULL || Enroll_Find(follower, currCourse->id) == NULL)
                    continue;
                else if (Student_IdNode_Add(currCourse->followed->first, crt_follower_id)) {
                    currCourse->followed->student_count++;
                }
            }
        }
    }
    cJSON_Delete(cjson_Data);
}

static void Student_Node_Free(Student_Node *res) {
    Student_Node *last = NULL, *crt = res;
    while (crt) {
        last = crt;
        Enroll *lastEnroll = NULL, *crtEnroll = crt->stu.enrolled;
        while (crtEnroll) {
            lastEnroll = crtEnroll;
            crtEnroll = crtEnroll->next;
            free(lastEnroll);
        }
        crt = crt->next;

        free(last);
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
        free(last_node);
    }
    free(stu_list);
}

static void Course_Node_Free(Course_Node *res) {
    Course_Node *last = NULL, *crt = res;
    while (crt) {
        last = crt;
        Student_List_Free(crt->crs.followed);
        crt = crt->next;
        free(last);
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
}
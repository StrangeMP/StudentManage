#include "StuMan_Import.h"
#include "StuMan_BuildIndex.h"
#include "StuMan_Node.h"
#include "StuMan_Search.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Enroll *Enroll_Construct(const cJSON *item);
static Enroll *Enroll_Append(Student *dest, cJSON *cjson_enrolled);
Student *Student_Insert(const cJSON *cjson_student);
Course *Course_Insert(const cJSON *cjson_course);
void ImportData(const char *fileDir);

static Enroll *Enroll_Construct(const cJSON *item) {
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
    newEnr->prev = NULL;
    newEnr->next = NULL;
    return newEnr;
}

// Adds cjson_enrolled to dest if possible, returns the added node if added, otherwise returns NULL.
// Course follower-list is checked and updated if possible, regardless of
// whether dest has enrolled in the course;
static Enroll *Enroll_Append(Student *dest, cJSON *cjson_enrolled) {
    if (dest == NULL || cjson_enrolled == NULL)
        return NULL;
    Enroll *ret = NULL;
    char course_id[13];
    strcpy(course_id, cJSON_GetObjectItem(cjson_enrolled, "course_id")->valuestring);
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
        Student_IdNode *idnode = stu_list->first;
        if (idnode == NULL) { // crs_dest followed-list empty, initialize a new node.
            stu_list->first = Student_IdNode_Add(NULL, dest->id);
            stu_list->student_count++;
        } else
            // Try add dest->id to crs_dest->followed
            // crs_dest->followed is guaranteed not to be empty at this point
            if (Student_IdNode_Add(idnode, dest->id))
                stu_list->student_count++;
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

    if (currStudent == NULL) {
        // id doesn't exist, construct a new Student and insert to database
        Student_Node *currStudentNode = (Student_Node *)malloc(sizeof(Student_Node));
        currStudentNode->prev = currStudentNode->next = NULL;
        data_address.pStudentFoot->next = currStudentNode;
        currStudentNode->prev = data_address.pStudentFoot;
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
        currCourseNode->prev = currCourseNode->next = NULL;
        data_address.pCourseFoot->next = currCourseNode;
        currCourseNode->prev = data_address.pCourseFoot;
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
            else if (Student_IdNode_Add(currCourse->followed->first, crt_follower_id))
                currCourse->followed->student_count++;
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

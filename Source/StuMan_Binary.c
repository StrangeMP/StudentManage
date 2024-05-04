#include "StuMan_Binary.h"
#include "StuMan_Benefit.h"
#include "StuMan_BuildIndex.h"
#include "StuMan_Memory.h"
#include "StuMan_Student.h"
#include <stdio.h>

enum sym { eof_sym, stu_sym, enr_sym, ess_sym, prj_sym, awd_sym, crs_sym };

static void Stu_Write(Student pStu, FILE *f);
void SaveData();
static void Cor_Write(Course pCor, FILE *f);
void LoadData(const char *StudentFile, const char *CourseFile);

void SaveData() {
    FILE *p = fopen("__Stu_Info.bin", "wb");
    FILE *f = fopen("__Cor_Info.bin", "wb"); // 学生数据一个文件，课程数据一个文件
    Student_Node *SHead = data_address.pStudentHead; // the pointer to move
    Course_Node *CHead = data_address.pCourseHead;   // the pointer to move

    while (SHead != NULL) // 先写学生的数据
    {
        Stu_Write(SHead->stu, p);
        SHead = SHead->next;
    }

    while (CHead != NULL) // 再写课程的数据
    {
        Cor_Write(CHead->crs, f);
        CHead = CHead->next;
    }

    fclose(p);
    fclose(f);
}

static void write_symbol(enum sym _symbol, FILE *f) {
    enum sym *_S = &_symbol;
    fwrite(_S, sizeof(enum sym), 1, f);
}

static void Stu_Write(Student pStu, FILE *f) {
    write_symbol(stu_sym, f);
    fwrite(&pStu, sizeof(Student), 1, f); // 录入学生信息

    // 录入课程
    Enroll *p0 = pStu.enrolled;
    while (p0 != NULL) {
        write_symbol(enr_sym, f);
        fwrite(p0, sizeof(Enroll), 1, f);
        p0 = p0->next;
    }

    // 录入素质加分
    // Essays
    Essay *essay0 = pStu.Benefits.essays;
    while (essay0 != NULL) {
        write_symbol(ess_sym, f);
        fwrite(essay0, sizeof(Essay), 1, f);
        essay0 = essay0->next;
    }

    // Project
    Project *project0 = pStu.Benefits.projects;
    while (project0 != NULL) {
        write_symbol(prj_sym, f);
        fwrite(project0, sizeof(Project), 1, f);
        project0 = project0->next;
    }

    // Award
    Award *award0 = pStu.Benefits.awards;
    while (award0 != NULL) {
        write_symbol(awd_sym, f);
        fwrite(award0, sizeof(Award), 1, f);
        award0 = award0->next;
    }
}

static void Cor_Write(Course pCor, FILE *f) {
    write_symbol(crs_sym, f);
    fwrite(&pCor, sizeof(Course), 1, f);                      // 录入整个课程
    fwrite(&pCor.followed->student_count, sizeof(int), 1, f); // 录入学生人数
    Student_IdNode *p0 = pCor.followed->first;
    while (p0 != NULL) {
        fwrite(&p0->id, sizeof(int), 1, f); // 录入学号
        p0 = p0->next;
    }
}

enum sym get_symbol(FILE *f) {
    enum sym _S;
    if (feof(f))
        _S = eof_sym;
    else
        fread(&_S, sizeof(_S), 1, f);
    return _S;
}

static void LoadStudents(const char *StuBinDir) {
    FILE *p = fopen(StuBinDir, "rb");

    // 存入学生链表
    Student_Node *ph = NULL, *pf = NULL, *p0 = NULL; // 头 尾
    ph = (Student_Node *)MALLOC(sizeof(Student_Node));
    ph->prev = NULL;
    ph->next = NULL; // 初始化
    pf = ph;
    enum sym crt_sym = get_symbol(p);
    while (crt_sym == stu_sym) {
        p0 = (Student_Node *)MALLOC(sizeof(Student_Node));
        fread(&p0->stu, sizeof(Student), 1, p);
        // 建立索引
        Build_Student_Index(p0);
        // 连接新学生节点
        p0->next = NULL;
        p0->prev = pf;
        pf->next = p0;
        pf = pf->next;
        ++data_address.student_total;
        // 本次录入结束，读取下一个标识
        crt_sym = get_symbol(p);
        if (crt_sym == stu_sym)
            continue;
        else {
            if (crt_sym == enr_sym) // 录入选课信息
            {
                Enroll *head = (Enroll *)MALLOC(sizeof(Enroll)), // 哨兵项
                    *rear = head;
                head->prev = NULL;
                head->next = NULL;
                // 写Enroll
                while (crt_sym == enr_sym) {
                    Enroll *enroll = (Enroll *)MALLOC(sizeof(Enroll));
                    fread(enroll, sizeof(Enroll), 1, p);
                    enroll->next = NULL;
                    rear->next = enroll;
                    enroll->prev = rear;
                    rear = rear->next;

                    // 本次录入结束，读取下一个标识
                    crt_sym = get_symbol(p);
                }
                p0->stu.enrolled = head->next;
                p0->stu.enrolled->prev = NULL;
                FREE(head); // 调整选课链表首项
            }

            if (crt_sym == ess_sym) { // 录入素质加分
                // Essay
                Essay *essayh = (Essay *)MALLOC(sizeof(Essay)), *essayr = essayh, *essay0 = NULL;
                essayh->next = NULL;
                essayh->prev = NULL;
                while (crt_sym == ess_sym) // 判断是否有essay
                {
                    essay0 = (Essay *)MALLOC(sizeof(Essay));
                    fread(essay0, sizeof(Essay), 1, p);
                    essay0->next = NULL;
                    essay0->prev = essayr;
                    essayr->next = essay0;
                    essayr = essayr->next;
                    // 本次录入结束
                    crt_sym = get_symbol(p);
                }
                p0->stu.Benefits.essays = essayh->next; // 给p0.Essay赋值
                if (p0->stu.Benefits.essays != NULL)
                    p0->stu.Benefits.essays->prev = NULL;
                FREE(essayh);
            }

            if (crt_sym == prj_sym) { // Project
                Project *projecth = (Project *)MALLOC(sizeof(Project)), *projectr = projecth,
                        *project0 = NULL;
                projecth->next = NULL;
                projecth->prev = NULL;
                while (crt_sym == prj_sym) // 判断是否有project
                {
                    project0 = (Project *)MALLOC(sizeof(Project));
                    fread(project0, sizeof(Project), 1, p);
                    project0->next = NULL;
                    project0->prev = projectr;
                    projectr->next = project0;
                    projectr = projectr->next;

                    crt_sym = get_symbol(p);
                }
                p0->stu.Benefits.projects = projecth->next; // 给p0.Project赋值
                if (p0->stu.Benefits.projects != NULL)
                    p0->stu.Benefits.projects->prev = NULL;
                FREE(projecth);
            }

            if (crt_sym == awd_sym) { // Award
                Award *awardh = (Award *)MALLOC(sizeof(Award)), *awardr = awardh, *award0 = NULL;
                awardh->next = NULL;
                awardh->prev = NULL;
                while (crt_sym == awd_sym) // 判断是否存在Award
                {
                    award0 = (Award *)MALLOC(sizeof(Award));
                    fread(award0, sizeof(Award), 1, p);
                    award0->next = NULL;
                    award0->prev = awardr;
                    awardr->next = award0;
                    awardr = awardr->next;

                    crt_sym = get_symbol(p);
                }
                p0->stu.Benefits.awards = awardh->next; // 给p0.Award赋值
                if (p0->stu.Benefits.awards != NULL)
                    p0->stu.Benefits.awards->prev = NULL;
                FREE(awardh);
            }
        }
    }
    data_address.pStudentHead = ph->next; // 第二项开始才是学生
    FREE(ph);
    if (data_address.pStudentHead)
        data_address.pStudentHead->prev = NULL; // 去除哨兵项
    data_address.pStudentFoot = pf;
    fclose(p);
}

static void LoadCourses(const char *CrsBinDir) {
    // 存入课程信息
    FILE *f = fopen(CrsBinDir, "rb");
    Course_Node *pCh, *pCf, *pC0; // 头 尾
    pCh = (Course_Node *)MALLOC(sizeof(Course_Node));
    pCf = pCh;
    pCh->prev = NULL;
    pCh->next = NULL; // 建立课程链表
    enum sym crt_sym = get_symbol(f);
    while (crt_sym == crs_sym) {
        pC0 = (Course_Node *)MALLOC(sizeof(Course_Node));
        // 建立课程的学生指针指向的Student_List结构体，否则不存在无法指向。
        Student_List *followStu = (Student_List *)MALLOC(sizeof(Student_List));
        fread(&(pC0->crs), sizeof(Course), 1, f);
        pC0->crs.followed = followStu;
        fread(&(pC0->crs.followed->student_count), sizeof(int), 1, f);

        Student_IdNode *fh, *fr, *f0; // 学号链表的建立
        fh = (Student_IdNode *)MALLOC(sizeof(Student_IdNode));
        fh->prev = NULL;
        fh->next = NULL;
        fr = fh;

        for (int i = 0; i < pC0->crs.followed->student_count; i++) {
            f0 = (Student_IdNode *)MALLOC(sizeof(Student_IdNode));
            fread(&f0->id, sizeof(int), 1, f);
            f0->next = NULL;
            f0->prev = fr;
            fr->next = f0;
            fr = fr->next;
        };

        // 将学号链表接到课程follower
        pC0->crs.followed->first = fh->next;
        FREE(fh);
        pC0->crs.followed->end = fr;

        pC0->next = NULL;
        pC0->prev = pCf;
        pCf->next = pC0;
        pCf = pCf->next;
        ++data_address.course_total;
        crt_sym = get_symbol(f);
    }

    data_address.pCourseHead = pCh->next;
    if (data_address.pCourseHead)
        data_address.pCourseHead->prev = NULL; // 去除哨兵项
    data_address.pCourseFoot = pCf;
    FREE(pCh);
    fclose(f);
}

void LoadData(const char *StudentFile, const char *CourseFile) {
    LoadStudents(StudentFile);
    LoadCourses(CourseFile);
    for (Student_Node *crt_node = data_address.pStudentHead; crt_node; crt_node = crt_node->next)
        PendingList_Update(&crt_node->stu);
}

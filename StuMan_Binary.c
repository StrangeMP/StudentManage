#include "StuMan_Binary.h"
#include "StuMan_Memory.h"
#include "StuMan_Student.h"
#include <stdio.h>

static char symbol[2] = {'<', '>'}; //'<'放在一个学生的开头；'>'放在一个学生的最后
static char symbool[2] = {'(', ')'}; //'('放在课程中学生学号的开头，')'放在一个课程的结束
static char benefit[3] = {'*', '^', '+'}; //'*'放在essay末尾，'^'放在project末尾，'+'放在award末尾

static void Stu_Write(Student pStu, FILE *f);
void SaveData();
static void Cor_Write(Course pCor, FILE *f);
void LoadData(const char *fileDir1, const char *fileDir2);
static bool examin(FILE *p, char a);

void SaveData() {
    FILE *p = fopen("Stu_Info.bin", "wb");
    FILE *f = fopen("Cor_Info.bin", "wb"); // 学生数据一个文件，课程数据一个文件
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

static void Stu_Write(Student pStu, FILE *f) {
    fwrite(symbol, sizeof(char), 1, f); // 录入‘<’代表学生录入开始
    // fwrite(&pStu.name,sizeof(char),32,f);          //录入学生名字
    // fwrite(&pStu.id,sizeof(int),1,f);              //录入学号
    // fwrite(&pStu.institute_grade,sizeof(int),1,f); //录入学院
    fwrite(&pStu, sizeof(Student), 1, f); // 录入学生信息
    // 录入课程
    Enroll *p0 = pStu.enrolled;
    while (p0 != NULL) {
        fwrite(p0, sizeof(Enroll), 1, f);
        p0 = p0->next;
    }
    fwrite(symbol + 1, sizeof(char), 1, f); // 录入‘>’代表课程录入结束

    // 录入素质加分

    // Essays
    Essay *essay0 = pStu.Benefits.essays;
    if (essay0 != NULL)
        fwrite(benefit, sizeof(char), 1, f);
    while (essay0 != NULL) {
        fwrite(essay0, sizeof(Essay), 1, f);
        essay0 = essay0->next;
    }

    // Project
    Project *project0 = pStu.Benefits.projects;
    if (project0 != NULL)
        fwrite(benefit + 1, sizeof(char), 1, f);
    while (project0 != NULL) {
        fwrite(project0, sizeof(Project), 1, f);
        project0 = project0->next;
    }

    // Award
    Award *award0 = pStu.Benefits.awards;
    if (award0 != NULL)
        fwrite(benefit + 2, sizeof(char), 1, f);
    while (award0 != NULL) {
        fwrite(award0, sizeof(Award), 1, f);
        award0 = award0->next;
    }
}

static void Cor_Write(Course pCor, FILE *f) {
    fwrite(symbool, sizeof(char), 1, f);                      // ‘(’代表课程开始录入
    fwrite(&pCor, sizeof(Course), 1, f);                      // 录入整个课程
    fwrite(&pCor.followed->student_count, sizeof(int), 1, f); // 录入学生人数
    Student_IdNode *p0 = pCor.followed->first;
    while (p0 != NULL) {
        fwrite(&p0->id, sizeof(int), 1, f); // 录入学号
        p0 = p0->next;
    }
    fwrite(symbool + 1, sizeof(char), 1, f); //')'代表学生录入结束
}

void LoadData(const char *fileDir1, const char *fileDir2) {
    FILE *p = fopen(fileDir1, "rb");

    // 存入学生链表
    Student_Node *ph, *pf, *p0; // 头 尾
    ph = (Student_Node *)MALLOC(sizeof(Student_Node));
    ph->prev = NULL;
    ph->next = NULL; // 初始化
    pf = ph;
    while (examin(p, '<')) {
        p0 = (Student_Node *)MALLOC(sizeof(Student_Node));
        // fread(&p0->stu.name,sizeof(char),32,p);
        // fread(&p0->stu.id,sizeof(int),1,p);
        // fread(&p0->stu.institute_grade,sizeof(int),1,p);
        fread(&p0->stu, sizeof(Student), 1, p);

        // 录入选课信息
        Enroll *head = (Enroll *)MALLOC(sizeof(Enroll)), // 哨兵项
            *rear = head;
        head->prev = NULL;
        head->next = NULL;
        do {
            Enroll *enroll = (Enroll *)MALLOC(sizeof(Enroll));
            fread(enroll, sizeof(Enroll), 1, p);
            enroll->next = NULL;
            rear->next = enroll;
            enroll->prev = rear;
            rear = rear->next;
        } while (!examin(p, '>'));
        p0->stu.enrolled = head->next;
        p0->stu.enrolled->prev = NULL;
        FREE(head); // 调整链表首项

        // 录入素质加分
        // Essay
        Essay *essayh = (Essay *)MALLOC(sizeof(Essay)), *essayr = essayh, *essay0 = NULL;
        essayh->next = NULL;
        essayh->prev = NULL;
        while (examin(p, '*')) // 判断是否有essay
        {
            essay0 = (Essay *)MALLOC(sizeof(Essay));
            fread(essay0, sizeof(Essay), 1, p);
            essay0->next = NULL;
            essay0->prev = essayr;
            essayr->next = essay0;
            essayr = essayr->next;
        }
        p0->stu.Benefits.essays = essayh->next; // 给p0.Essay赋值
        if (p0->stu.Benefits.essays != NULL)
            p0->stu.Benefits.essays->prev = NULL;

        // Project
        Project *projecth = (Project *)MALLOC(sizeof(Project)), *projectr = projecth,
                *project0 = NULL;
        projecth->next = NULL;
        projecth->prev = NULL;
        while (examin(p, '^')) // 判断是否有project
        {
            project0 = (Project *)MALLOC(sizeof(Project));
            fread(project0, sizeof(Project), 1, p);
            project0->next = NULL;
            project0->prev = projectr;
            projectr->next = project0;
            projectr = projectr->next;
        }
        p0->stu.Benefits.projects = projecth->next; // 给p0.Project赋值
        if (p0->stu.Benefits.projects != NULL)
            p0->stu.Benefits.projects->prev = NULL;

        // Award
        Award *awardh = (Award *)MALLOC(sizeof(Award)), *awardr = awardh, *award0 = NULL;
        awardh->next = NULL;
        awardh->prev = NULL;
        while (examin(p, '+')) // 判断是否存在Award
        {
            award0 = (Award *)MALLOC(sizeof(Award));
            fread(award0, sizeof(Award), 1, p);
            award0->next = NULL;
            award0->prev = awardr;
            awardr->next = award0;
            awardr = awardr->next;
        }
        p0->stu.Benefits.awards = awardh->next; // 给p0.Award赋值
        if (p0->stu.Benefits.awards != NULL)
            p0->stu.Benefits.awards->prev = NULL;

        p0->next = NULL;
        p0->prev = pf;
        pf->next = p0;
        pf = pf->next;
    }
    data_address.pStudentHead = ph->next; // 第二项开始才是学生
    FREE(ph);
    data_address.pStudentHead->prev = NULL; // 去除哨兵项
    data_address.pStudentFoot = pf;

    // 存入课程信息
    FILE *f = fopen(fileDir2, "rb");
    Course_Node *pCh, *pCf, *pC0; // 头 尾
    pCh = (Course_Node *)MALLOC(sizeof(Course_Node));
    pCf = pCh;
    pCh->prev = NULL;
    pCh->next = NULL; // 建立课程链表
    while (examin(f, '(')) {
        pC0 = (Course_Node *)MALLOC(sizeof(Course_Node));
        fread(&pC0->crs, sizeof(Course), 1, f);
        fread(&pC0->crs.followed->student_count, sizeof(int), 1, f);

        Student_IdNode *fh, *fr, *f0; // 学号链表的建立
        fh = (Student_IdNode *)MALLOC(sizeof(Student_IdNode));
        fh->prev = NULL;
        fh->next = NULL;
        fr = fh;

        do {
            f0 = (Student_IdNode *)MALLOC(sizeof(Student_IdNode));
            fread(&f0->id, sizeof(int), 1, f);
            f0->next = NULL;
            f0->prev = fr;
            fr->next = f0;
            fr = fr->next;
        } while (!examin(f, ')'));

        pC0->crs.followed->first = fh->next;
        FREE(fh);
        pC0->crs.followed->first = fr;

        pC0->next = NULL;
        pCf->next = pC0;
        pC0->prev = pCf;
        pCf = pCf->next;
    }
    data_address.pCourseHead = pCh->next;
    // data_address.pCourseHead->prev=NULL;//去除哨兵项
    data_address.pCourseFoot = pCf;

    fclose(p);
    fclose(f);
}

static bool examin(FILE *p, char a) {
    char exam;
    fread(&exam, sizeof(char), 1, p);
    if (exam == a)
        return true;
    else {
        fseek(p, (-1) * (long long)sizeof(char), SEEK_CUR);
        return false;
    }
}
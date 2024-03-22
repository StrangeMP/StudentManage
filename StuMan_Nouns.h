#pragma once

extern const char *Institutes[];
extern const char *Professions[88][15];
extern const char *pubCourseCategory[];
extern const char *Course_type[];
extern const char *Enroll_retake[];
extern const char *Enroll_level[];
extern const char *Course_nature[];
extern const char *Course_grade_type[];
extern const char *Course_exam_type[];
extern const char *Benefit_Status[];
int Get_NounArrLen(const char **p);
int getNounIndex(const char **nounArr, int arrLen, const char *content);
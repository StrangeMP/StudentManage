#pragma once
#include "StuMan_Student.h"
#include "cJSON.h"
/*
Export Students and Courses recorded in _data to _dest.

_data should be formatted to:
{
    "Students": [23220652, 21230254, 22210267],
    "Courses": ["gx50902530", "gx50942603"]
}
*/
void ExportData(cJSON *_data, const char *fileName);

cJSON *CreateExportList(int stuArr[], const int stuCnt, char *crsArr[], const int crsCnt);
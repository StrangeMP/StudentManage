#pragma once
#include "cJSON.h"
/*
Export Students and Courses recorded in _data to _dest.
If fileName is specified, write exported result to the file, otherwise returns the exported result.
_data should be formatted as:
{
    "Students": [23220652, 21230254, 22210267],
    "Courses": ["gx50902530", "gx50942603"]
}
*/
cJSON *ExportData(cJSON *_data, const char *fileName);

cJSON *CreateExportList(int stuArr[], const int stuCnt, const char *crsArr[], const int crsCnt);
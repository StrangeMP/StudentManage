#include "StuMan_Export.h"
#include "StuMan_Import.h"
#include <stdio.h>

int main() {
    ImportData("TestData_Benefits.json");
    int stu_arr[5] = {23230625, 12211024, 33201920, 25220320, 30201920};
    char *crs_arr[2] = {"gx50983009", "gx50944301"};
    cJSON *tobeexport = CreateExportList(stu_arr, 5, crs_arr, 2);
    // char *output = cJSON_Print(tobeexport);
    // FILE *f = fopen("__Exported.json", "w");
    // fprintf(f, "%s", output);
    ExportData(tobeexport, "__Exported.json");
    ReleaseResource();
    return 0;
}
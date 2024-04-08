#include "StuMan_Binary.h"
#include "StuMan_Import.h"

int main() {
    ImportData("TestData_Benefits.json");
    SaveData();
    ReleaseResource();
    return 0;
}
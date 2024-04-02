#include "StuMan_Import.h"
#include "StuMan_Server.h"
int main() {
    ImportData("TestData_Benefits.json");
    RunServer();
    ReleaseResource();
    return 0;
}
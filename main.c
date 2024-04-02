#include "StuMan_Account.h"
#include "StuMan_Import.h"
#include "StuMan_Server.h"
int main() {
    ImportData("TestData_Benefits.json");
    Teacher_Signup("Teachers.json");
    RunServer();
    ReleaseResource();
    return 0;
}
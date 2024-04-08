#include "StuMan_Account.h"
#include "StuMan_Binary.h"
#include "StuMan_Server.h"

int main(int argc, char **argv) {
    if (argc != 3)
        LoadData("__Stu_Info.bin", "__Cor_Info.bin");
    else
        LoadData(argv[1], argv[2]);
    Teacher_Signup("Teachers.json");
    RunServer();
    SaveData();
    ReleaseResource();
    return 0;
}
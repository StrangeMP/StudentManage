#include "StuMan_Binary.h"
#include "StuMan_Server.h"

int main() {
    LoadData("./Storage/Stu_Info.bin", "./Storage/Cor_Info.bin");
    RunServer();
    SaveData();
    ReleaseResource();
    return 0;
}
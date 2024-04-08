#include "StuMan_Binary.h"
#include "StuMan_Server.h"

int main() {
    LoadData("__Stu_Info.bin", "__Cor_Info.bin");
    RunServer();
    SaveData();
    ReleaseResource();
    return 0;
}
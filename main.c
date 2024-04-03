#include "StuMan_Binary.h"
#include "StuMan_Server.h"

int main() {
    LoadData("Stu_Info.bin", "Cor_Info.bin");
    RunServer();
    SaveData();
    ReleaseResource();
    return 0;
}
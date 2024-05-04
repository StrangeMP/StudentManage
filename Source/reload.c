#include "StuMan_Binary.h"
#include "StuMan_Memory.h"
#include "StuMan_Import.h"

int main(int argc, char **argv) {
    if (argc != 2)
        ImportData("TestData.json");
    else
        ImportData(argv[1]);
    SaveData();
    ReleaseResource();
    return 0;
}
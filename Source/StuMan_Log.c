#include <stdio.h>
#include <time.h>
#include "StuMan_Log.h"

void Log(const char *request) {
    FILE *logFile = fopen("__log.txt", "a");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(logFile, "\n%d-%02d-%02d %02d:%02d:%02d Received:\n%s\n", tm.tm_year + 1900,
            tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, request);
    fclose(logFile);
    return;
}
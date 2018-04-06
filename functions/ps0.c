#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include "general.h"
//------------------------------------------------------------------------------
// CON CAT DIR
//------------------------------------------------------------------------------
void concatDir(char* path, const char* dir, const char* father) {
    size_t size = strlen(dir) + strlen(father) + strlen("/") + 1;
    snprintf(path, size, "%s/%s", father, dir);
}
//------------------------------------------------------------------------------
// GET FATHER
//------------------------------------------------------------------------------
void getFather(char* father, const char* path) {
    char s = '0';
    size_t pos = strlen(path) - 1;
    size_t size = strlen(path) - 1;
    while (s != '/') {
        father[size - pos] = path[pos];
        pos--;
        s = path[pos];
    }
    father[size - pos] = '\0';
}
//------------------------------------------------------------------------------
// PRINT PROC
//------------------------------------------------------------------------------
int printProc(const char* file, const char* father) {
    if (strncmp(file, "comm", 4) != 0) return 0;
    char path[256];
    concatDir(path, file, father);
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perr("ERROR at opening file: %s in function printProc()", file);
        return 1;
    }
    char proc[256];
    ssize_t read = readArchive(fd, proc, 256);
    proc[read] = '\0';
    if (read == -1) return 1;
    char msg[256];
    char pid[5];
    getFather(pid, father);
    size_t size = strlen(pid) + (read) + strlen(" ") + 1;
    snprintf(msg, size, "%s %s\n", pid, proc);
    writeArchive(STDOUT_FILENO, msg, size);
    if (close(fd) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// IS PID
//------------------------------------------------------------------------------
int isPid(const char* pid, const char* father) {
    size_t size = strlen(pid);
    for (size_t i = 0; i < size; i++) {
        if (!isdigit(pid[i])) return 0;
    }
    char path[256];
    concatDir(path, pid, father);
    return walk(path, printProc);
}
//------------------------------------------------------------------------------
// PS0
//------------------------------------------------------------------------------
int ps0() {
    return walk("/proc", isPid);
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return ps0();
}
//------------------------------------------------------------------------------

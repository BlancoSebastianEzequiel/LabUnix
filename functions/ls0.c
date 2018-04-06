#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "general.h"
//------------------------------------------------------------------------------
// WRITE ARCHIVES
//------------------------------------------------------------------------------
int writeFunctions(const char* dir, const char* father) {
    struct stat buffer;
    if (stat(dir, &buffer) == -1) {
        perr("ERROR with syscall stat()");
        return 1;
    }
    if ((buffer.st_mode & S_IFMT) != S_IFREG) return 0;
    size_t size = strlen(dir);
    writeArchive(STDOUT_FILENO, (void*) dir, size);
    writeArchive(STDOUT_FILENO, "\n", 2);
    return 0;
}
//------------------------------------------------------------------------------
// LS0
//------------------------------------------------------------------------------
int ls0() {
    return walk("./", writeFunctions);
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return ls0();
}
//------------------------------------------------------------------------------
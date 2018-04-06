#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#include <stdio.h>
#include "general.h"
//------------------------------------------------------------------------------
// TOUCH1
//------------------------------------------------------------------------------
int touch1(const char* file) {
    //Pre: si el archivo existe, es un archivo regular.
    int fd = open(file, O_WRONLY);
    if (fd == -1) {
        fd = open(file, O_CREAT, S_IRWXU);
    }
    else {
        if (utime(file, NULL) == -1) {
            perr("ERROR with syscall utime with file: %s", file);
            return 1;
        }
    }
    if (close(fd) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return touch1(argv[1]);
}
//------------------------------------------------------------------------------

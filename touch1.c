#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#include <stdio.h>

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
        int value = utime(file, NULL);
        if (value == -1) {
            perror("ERROR at function touch0 with utime");
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

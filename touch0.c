#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
//------------------------------------------------------------------------------
// TOUCH0
//------------------------------------------------------------------------------
int touch0(const char* file) {
    //Pre: si el archivo existe, es un archivo regular.
    int fd = open(file, O_WRONLY);
    if (fd == -1) {
        fd = open(file, O_CREAT, S_IRWXU);
    }
    if (close(fd) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return touch0(argv[1]);
}
//------------------------------------------------------------------------------

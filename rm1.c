#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <string.h>
//------------------------------------------------------------------------------
// RM1
//------------------------------------------------------------------------------
int rm1(const char* file) {
    //Pre: el archivo existe, y es un directorio o un archivo regular.
    if (unlink(file) == -1) {
        int bytes = 0;
        char msg[256];
        size_t size = strlen("cannot remove ") + strlen(file) + 1;
        snprintf(msg+bytes, size,"cannot remove %s", file);
        perror(msg);
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return rm1(argv[1]);
}
//------------------------------------------------------------------------------

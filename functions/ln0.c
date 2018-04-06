#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
//------------------------------------------------------------------------------
// LN0
//------------------------------------------------------------------------------
int ln0(const char *target, const char *linkPath) {
    // Pre: no existe un archivo con el nombre del enlace.
    if (symlink(target, linkPath) == -1) {
        perror("ERROR with syscall symlink()");
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return ln0(argv[1], argv[2]);
}
//------------------------------------------------------------------------------

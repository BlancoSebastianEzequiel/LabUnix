#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "general.h"
//------------------------------------------------------------------------------
// RM1
//------------------------------------------------------------------------------
int rm1(const char* file) {
    //Pre: el archivo existe, y es un directorio o un archivo regular.
    if (unlink(file) == -1) {
        perr("cannot remove %s", file);
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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "general.h"
//------------------------------------------------------------------------------
// MV0
//------------------------------------------------------------------------------
int mv0(const char *file, const char *copy) {
    // Pre: el archivo destino no existe.
    char newPath[256], oldPath[256];

    size_t newSize = strlen("./") + strlen(copy) + 1;
    snprintf(newPath, newSize,"./%s", copy);

    size_t oldSize = strlen("./") + strlen(file) + 1;
    snprintf(oldPath, oldSize,"./%s", file);

    if (rename(oldPath, newPath) == -1) {
        perr("ERROR: syscall rename. oldpath: %s. newpath: %s",
             oldPath, newPath);
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return mv0(argv[1], argv[2]);
}
//------------------------------------------------------------------------------

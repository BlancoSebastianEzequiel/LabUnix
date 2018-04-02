#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

    int value = rename(oldPath, newPath);
    if (value == -1) {
        perror("ERROR with function rename()");
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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
//------------------------------------------------------------------------------
// MV0
//------------------------------------------------------------------------------
int mv0(const char *file, const char *directory) {
    // Pre: el archivo destino no existe.
    int bytes = 0;
    char newPath[256], oldPath[256];
    size_t newSize = strlen("./") + strlen(directory) + strlen(file) + 1;
    snprintf(newPath+bytes, newSize,"./%s%s", directory, file);
    size_t oldSize = strlen("./") + strlen(file) + 1;
    bytes = 0;
    snprintf(oldPath+bytes, oldSize,"./%s", file);
    printf("oldPath: %s - newPath: %s\n", oldPath, newPath);
    int value = rename(oldPath, newPath);

    if (value == -1) {
        perror("ERROR");
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

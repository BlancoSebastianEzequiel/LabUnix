#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "functions.h"
//------------------------------------------------------------------------------
// CP0
//------------------------------------------------------------------------------
int cp0(const char* file, const char* copy) {
    // Pre: el archivo de origen existe y es regular.
    // El archivo destino no existe.
    int fdFile = open(file, O_RDONLY);
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;  // 0644
    int fdCopy = open(copy, O_WRONLY|O_CREAT, mode);
    ssize_t read = 1, written = 1;
    char buffer[256];
    while (read != 0 && written != 0) {
        read = readArchive(fdFile, buffer, 256);
        written = writeArchive(fdCopy, buffer, (size_t) read);
        if (written == -1 || read == -1) return 1;
    }
    if (close(fdFile) == -1 || close(fdCopy) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return cp0(argv[1], argv[2]);
}
//------------------------------------------------------------------------------

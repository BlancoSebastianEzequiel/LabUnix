#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "general.h"
//------------------------------------------------------------------------------
// TEE0
//------------------------------------------------------------------------------
int tee0(const char* file) {
    // Pre: el archivo o bien no existe, o bien es un archivo regular.
    char buffer[256];
    ssize_t read = 1, writtenSTD = 1, written = 1;
    int fdIn = fileno(stdin);
    int fdOut = fileno(stdout);
    int fdFile = open(file, O_WRONLY);
    if (fdFile == -1) {
        int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;  // 0644
        fdFile = open(file, O_WRONLY | O_CREAT, mode);
    }
    while (read != 0 && written != 0 && writtenSTD != 0) {
        read = readArchive(fdIn, buffer, 256);
        written = writeArchive(fdFile, buffer, (size_t) read);
        writtenSTD = writeArchive(fdOut, buffer, (size_t) read);
        if (read == -1 || written == -1 || writtenSTD == -1) {
            perr("Error while reading or writing");
            return 1;
        }
    }
    if (close(fdIn) == -1 || close(fdFile) == -1 || close(fdOut) == -1) {
        perr("Error while closing file");
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return tee0(argv[1]);
}
//------------------------------------------------------------------------------

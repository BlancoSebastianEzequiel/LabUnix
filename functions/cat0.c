#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "functions.h"
//------------------------------------------------------------------------------
// CAT 0
//------------------------------------------------------------------------------
int cat0(const char* pathName) {
    //pre: solo se pasa un archivo, este archivo existe
    // y se tienen permisos de lectura.
    char buffer[256];
    int fdOut = fileno(stdout);
    int fd = open(pathName, O_RDONLY);
    ssize_t read = 1, written = 1;
    while (read != 0 && written != 0) {
        read = readArchive(fd, buffer, 256);
        written = writeArchive(fdOut, buffer, (size_t) read);
        if (written == -1 || read == -1) return 1;
    }
    if (close(fd) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return cat0(argv[1]);
}
//------------------------------------------------------------------------------

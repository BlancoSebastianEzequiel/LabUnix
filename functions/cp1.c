#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include "general.h"
//------------------------------------------------------------------------------
// FILE SIZE
//------------------------------------------------------------------------------
size_t fileSize(const char* fileName) {
    struct stat st;
    stat(fileName, &st);
    return (size_t) st.st_size;
}
//------------------------------------------------------------------------------
// CP1
//------------------------------------------------------------------------------
int cp1(const char* file, const char* copy) {
    // Pre: el archivo de origen existe y es regular.
    // El archivo destino no existe.
    int fdFile = open(file, O_RDONLY);
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;  // 0644
    int fdCopy = open(copy, O_RDWR | O_CREAT, mode);
    if (fdCopy == -1) {
        perr("ERROR: file: %s at syscall open", copy);
        return 1;
    }

    size_t length = fileSize(file);
    if (truncate(copy, length) == -1) {
        perr("ERROR: with function truncate with file: %s", copy);
        return 1;
    }
    if (length == 0) {
        perr("ERROR: File is empty, nothing to do");
        return 1;
    }
    void *mappedAreaFile;
    void *mappedAreaCopy;

    mappedAreaFile = mmap(0, length, PROT_READ, MAP_SHARED, fdFile, 0);
    if (mappedAreaFile == MAP_FAILED) {
        perr("Error mmapping the stdin file");
        return 1;
    }

    mappedAreaCopy = mmap(0, length, PROT_WRITE, MAP_SHARED, fdCopy, 0);
    if (mappedAreaCopy == MAP_FAILED) {
        perr("Error mmapping the stdout file");
        return 1;
    }

    memcpy(mappedAreaCopy, mappedAreaFile, length);

    if (close(fdFile) == -1 || close(fdCopy) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return cp1(argv[1], argv[2]);
}
//------------------------------------------------------------------------------

#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include "general.h"
//------------------------------------------------------------------------------
// TOUCH0
//------------------------------------------------------------------------------
int stat0(const char* file) {
    //Pre: el archivo existe, y es un directorio o un archivo regular.
    struct stat buffer;
    int fd = open(file, O_RDONLY);
    if (stat(file, &buffer) == -1) {
        perr("ERROR with syscall stat()");
        return 1;
    }
    off_t st_size = buffer.st_size;
    char* mode = "";
    switch (buffer.st_mode & S_IFMT) {
        case S_IFSOCK:
            mode = "socket";
            break;
        case S_IFLNK:
            mode = "symbolic link";
            break;
        case S_IFREG:
            mode = "regular file";
            break;
        case S_IFBLK:
            mode = "block device";
            break;
        case S_IFDIR:
            mode = "directory";
            break;
        case S_IFCHR:
            mode = "character device";
            break;
        case S_IFIFO:
            mode = "FIFO";
            break;
        default:
            printf("ERROR: el tipo de archivo no se encontro");
            break;
    }
    printf("Size: %d\nFile: %s\nType: %s\n", (int) st_size, file, mode);
    if (close(fd) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return stat0(argv[1]);
}
//------------------------------------------------------------------------------

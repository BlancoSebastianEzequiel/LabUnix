#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
//------------------------------------------------------------------------------
// STAT0
//------------------------------------------------------------------------------
int stat0(const char* file) {
    //Pre: el archivo existe, y es un directorio o un archivo regular.
    //Pos: $./stat0 README.md
    //Size: 1318
    //File: README.md
    //Type: regular file
    struct stat buffer;
    int fd = open(file, O_RDONLY);
    int value = stat(file, &buffer);
    if (value == -1) {
        printf("ERROR at function stat0: %d\n", errno);
        return value;
    }
    // Total size, in bytes
    off_t st_size = buffer.st_size;
    // Protection
    /*
        S_IFMT     0170000   bit mask for the file type bit field

        S_IFSOCK   0140000   socket
        S_IFLNK    0120000   symbolic link
        S_IFREG    0100000   regular file
        S_IFBLK    0060000   block device
        S_IFDIR    0040000   directory
        S_IFCHR    0020000   character device
        S_IFIFO    0010000   FIFO

     */
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

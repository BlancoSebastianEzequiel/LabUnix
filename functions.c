#include "functions.h"
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
//------------------------------------------------------------------------------
// READ ARCHIVE
//------------------------------------------------------------------------------
ssize_t readArchive(int fd, void *buf, size_t bytes) {
    size_t bytesRead = 0;
    ssize_t value;
    while (bytesRead < bytes) {
        value = read(fd, buf+bytesRead, bytes - bytesRead);
        if (value == -1) {
            perror("ERROR at function readArchive");
            return value;
        }
        if (value == 0) {
            return bytesRead;
        }
        bytesRead += value;
    }
    return bytesRead;
}
//------------------------------------------------------------------------------
// WRITE ARCHIVE
//------------------------------------------------------------------------------
ssize_t writeArchive(int fd, void *buf, size_t bytes) {
    size_t bytesWritten = 0;
    ssize_t value;
    while (bytesWritten < bytes) {
        value = write(fd, buf+bytesWritten, bytes - bytesWritten);
        if (value == -1) {
            perror("ERROR at function writeArchive");
            return value;
        }
        if (value == 0) {
            return bytesWritten;
        }
        bytesWritten += value;
    }
    return bytesWritten;
}
//------------------------------------------------------------------------------
// IS ERROR
//------------------------------------------------------------------------------
void perr(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char msgError[BUF_LEN];
    vsnprintf(msgError, BUF_LEN, format, args);
    va_end(args);
    perror(msgError);
}
//------------------------------------------------------------------------------
// NEXT
//------------------------------------------------------------------------------
size_t next(struct dirent** direntStructure, DIR* directoryStream) {
    errno = 0;
    (*direntStructure) = readdir(directoryStream);
    if (*direntStructure == NULL && errno != 0) {
        perr("ERROR in function readdir()");
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
// WALK
//------------------------------------------------------------------------------
int walk(const char *dir, int (*f)(const char* a, const char* father)) {
    DIR* directoryStream = opendir(dir);
    if (directoryStream == NULL) {
        perr("ERROR with dir: %s in function opendir()", dir);
        return 1;
    }
    struct dirent* direntStructure;
    if(next(&direntStructure, directoryStream) == 1) return 1;
    while (direntStructure != NULL) {
        if (f(direntStructure->d_name, dir) == 1) return 1;
        if(next(&direntStructure, directoryStream) == 1) return 1;
    }
    if (closedir(directoryStream) == -1) {
        perror("ERROR WITH closedir");
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------

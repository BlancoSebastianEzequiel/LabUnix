#ifndef LAB1_SYSCALLS_H
#define LAB1_SYSCALLS_H
//------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------
#include <unistd.h>
#include <dirent.h>
#define ERROR 1
#define SUCCESS 0
#define BUF_LEN 256
//------------------------------------------------------------------------------
// READ ARCHIVE
//------------------------------------------------------------------------------
ssize_t readArchive(int fd, void *buf, size_t bytes);
//------------------------------------------------------------------------------
// WRITE ARCHIVE
//------------------------------------------------------------------------------
ssize_t writeArchive(int fd, void *buf, size_t bytes);
//------------------------------------------------------------------------------
// IS ERROR (PUNTEROS)
//------------------------------------------------------------------------------
void perr(const char *format, ...);
//------------------------------------------------------------------------------
// NEXT
//------------------------------------------------------------------------------
size_t next(struct dirent** direntStructure, DIR* directoryStream);
//------------------------------------------------------------------------------
// WALK
//------------------------------------------------------------------------------
int walk(const char *dir, int (*f)(const char* a, const char* father));
//------------------------------------------------------------------------------
#endif // LAB1_SYSCALLS_H

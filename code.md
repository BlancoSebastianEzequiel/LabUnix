# cat0.c
```
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "general.h"
//------------------------------------------------------------------------------
// CAT 0
//------------------------------------------------------------------------------
int cat0(const char* pathName) {
    //pre: solo se pasa un archivo, este archivo existe y se tienen permisos
    // de lectura.
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
  
```
# cp0.c
```
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "general.h"
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
  
```
# cp1.c
```
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
  
```
# general.c
```
#include "general.h"
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
// PERR
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
int walk(const char *dir, int (*f)(const char*, const char*)) {
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
  
```
# general.h
```
#ifndef LAB1_SYSCALLS_H
#define LAB1_SYSCALLS_H
//------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------
#include <unistd.h>
#include <dirent.h>
#define BUF_LEN 256
//------------------------------------------------------------------------------
// READ ARCHIVE
//------------------------------------------------------------------------------
// Pre: Recibo un file descriptor de un archivo, un buffer con un mensaje y
// el tamaño de byte a leer.
// Pos: Leo del archivo y guardo el mensaje en el buffer
ssize_t readArchive(int fd, void *buf, size_t bytes);
//------------------------------------------------------------------------------
// WRITE ARCHIVE
//------------------------------------------------------------------------------
// Pre: Recibo un file descriptor de un archivo, un buffer con un mensaje y
// el tamaño de byte a escribir.
// Pos: escribo en el archivo
ssize_t writeArchive(int fd, void *buf, size_t bytes);
//------------------------------------------------------------------------------
// IS ERROR (PUNTEROS)
//------------------------------------------------------------------------------
// Pre: Recibo un formato y los argumentos
// Pos: Escribo el mensaje por pantalla y se muestra el mensaje de error
// correspondiente a la variable errno
void perr(const char *format, ...);
//------------------------------------------------------------------------------
// NEXT
//------------------------------------------------------------------------------
// Pre: Recibo la esctructura que me dice el archivo o directorio y
// recibo el directorio que se abrio previamente.
// Pos: retorna 0 si la estructura siguiente existe, es decir, no es NULL.
// Si es NULL y se seteo el valor de errno entonces hubo un error
// y se retorna 1 pero si es NULL pero no se sete el valor de errno entonces
// significa que llegue al ultimo archivo o directorio del directiorio
// el cual estoy reccoriendo y devuelvo 0.
size_t next(struct dirent** direntStructure, DIR* directoryStream);
//------------------------------------------------------------------------------
// WALK
//------------------------------------------------------------------------------
// Pre: Recibo el nombre de una direccion la cual se quiere recorrer y una
// funcion la cual recibe dos cadenas que podrian ser una nueva direccion y
// la direccion padre.
// Pos: Devuelve 1 si hubo algun tipo de error con las distintas funciones
// que se usas y se muestra un mensaje mostrando que paso. Se retorna 0
// si el recorrido de la direccion recibida se completo esitosamente.
int walk(const char *dir, int (*f)(const char* a, const char* father));
//------------------------------------------------------------------------------
#endif // LAB1_SYSCALLS_H
  
```
# ln0.c
```
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
//------------------------------------------------------------------------------
// LN0
//------------------------------------------------------------------------------
int ln0(const char *target, const char *linkPath) {
    // Pre: no existe un archivo con el nombre del enlace.
    if (symlink(target, linkPath) == -1) {
        perror("ERROR with syscall symlink()");
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return ln0(argv[1], argv[2]);
}
//------------------------------------------------------------------------------
  
```
# ln1.c
```
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
//------------------------------------------------------------------------------
// LN1
//------------------------------------------------------------------------------
int ln1(const char *target, const char *linkPath) {
    // Pre: no existe un archivo con el nombre del enlace.
    if (link(target, linkPath) == -1) {
        perror("ERROR with syscall link");
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return ln1(argv[1], argv[2]);
}
//------------------------------------------------------------------------------
  
```
# ls0.c
```
#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "general.h"
//------------------------------------------------------------------------------
// WRITE ARCHIVES
//------------------------------------------------------------------------------
int writeFunctions(const char* dir, const char* father) {
    struct stat buffer;
    if (stat(dir, &buffer) == -1) {
        perr("ERROR with syscall stat()");
        return 1;
    }
    if ((buffer.st_mode & S_IFMT) != S_IFREG) return 0;
    size_t size = strlen(dir);
    writeArchive(STDOUT_FILENO, (void*) dir, size);
    writeArchive(STDOUT_FILENO, "\n", 2);
    return 0;
}
//------------------------------------------------------------------------------
// LS0
//------------------------------------------------------------------------------
int ls0() {
    return walk("./", writeFunctions);
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return ls0();
}
//------------------------------------------------------------------------------  
```
# mv0.c
```
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "general.h"
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

    if (rename(oldPath, newPath) == -1) {
        perr("ERROR: syscall rename. oldpath: %s. newpath: %s",
             oldPath, newPath);
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
  
```
# ps0.c
```
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include "general.h"
//------------------------------------------------------------------------------
// CON CAT DIR
//------------------------------------------------------------------------------
void concatDir(char* path, const char* dir, const char* father) {
    size_t size = strlen(dir) + strlen(father) + strlen("/") + 1;
    snprintf(path, size, "%s/%s", father, dir);
}
//------------------------------------------------------------------------------
// GET FATHER
//------------------------------------------------------------------------------
void getFather(char* father, const char* path) {
    char s = '0';
    size_t pos = strlen(path) - 1;
    size_t size = strlen(path) - 1;
    while (s != '/') {
        father[size - pos] = path[pos];
        pos--;
        s = path[pos];
    }
    father[size - pos] = '\0';
}
//------------------------------------------------------------------------------
// PRINT PROC
//------------------------------------------------------------------------------
int printProc(const char* file, const char* father) {
    if (strncmp(file, "comm", 4) != 0) return 0;
    char path[256];
    concatDir(path, file, father);
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perr("ERROR at opening file: %s in function printProc()", file);
        return 1;
    }
    char proc[256];
    ssize_t read = readArchive(fd, proc, 256);
    proc[read] = '\0';
    if (read == -1) return 1;
    char msg[256];
    char pid[5];
    getFather(pid, father);
    size_t size = strlen(pid) + (read) + strlen(" ") + 1;
    snprintf(msg, size, "%s %s\n", pid, proc);
    writeArchive(STDOUT_FILENO, msg, size);
    if (close(fd) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// IS PID
//------------------------------------------------------------------------------
int isPid(const char* pid, const char* father) {
    size_t size = strlen(pid);
    for (size_t i = 0; i < size; i++) {
        if (!isdigit(pid[i])) return 0;
    }
    char path[256];
    concatDir(path, pid, father);
    return walk(path, printProc);
}
//------------------------------------------------------------------------------
// PS0
//------------------------------------------------------------------------------
int ps0() {
    return walk("/proc", isPid);
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return ps0();
}
//------------------------------------------------------------------------------
  
```
# rm0.c
```
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
//------------------------------------------------------------------------------
// RM0
//------------------------------------------------------------------------------
void rm0(const char* file) {
    //Pre: el archivo existe y es regular.
    unlink(file);
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    rm0(argv[1]);
    return 0;
}
//------------------------------------------------------------------------------
  
```
# rm1.c
```
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "general.h"
//------------------------------------------------------------------------------
// RM1
//------------------------------------------------------------------------------
int rm1(const char* file) {
    //Pre: el archivo existe, y es un directorio o un archivo regular.
    if (unlink(file) == -1) {
        perr("cannot remove %s", file);
        return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return rm1(argv[1]);
}
//------------------------------------------------------------------------------
  
```
# stat0.c
```
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
  
```
# tee0.c
```
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
  
```
# touch0.c
```
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
//------------------------------------------------------------------------------
// TOUCH0
//------------------------------------------------------------------------------
int touch0(const char* file) {
    //Pre: si el archivo existe, es un archivo regular.
    int fd = open(file, O_WRONLY);
    if (fd == -1) {
        fd = open(file, O_CREAT, S_IRWXU);
    }
    if (close(fd) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return touch0(argv[1]);
}
//------------------------------------------------------------------------------
  
```
# touch1.c
```
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#include <stdio.h>
#include "general.h"
//------------------------------------------------------------------------------
// TOUCH1
//------------------------------------------------------------------------------
int touch1(const char* file) {
    //Pre: si el archivo existe, es un archivo regular.
    int fd = open(file, O_WRONLY);
    if (fd == -1) {
        fd = open(file, O_CREAT, S_IRWXU);
    }
    else {
        if (utime(file, NULL) == -1) {
            perr("ERROR with syscall utime with file: %s", file);
            return 1;
        }
    }
    if (close(fd) == -1) return 1;
    return 0;
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    return touch1(argv[1]);
}
//------------------------------------------------------------------------------
  
```

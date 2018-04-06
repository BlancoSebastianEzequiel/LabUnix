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

#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
/*¿Cuál es la diferencia entre un hard link y un soft link?
 * Crear un hard link a un archivo, luego eliminar el archivo original
 * ¿Qué pasa con el enlace? ¿Se perdieron los datos del archivo?
 * Repetir lo mismo, pero con un soft link.
 * ¿Qué pasa ahora con el enlace? ¿Se perdieron los datos esta vez?
 * Explicar las diferencias.
 */
//------------------------------------------------------------------------------
// LN1
//------------------------------------------------------------------------------
int ln1(const char *target, const char *linkPath) {
    // Pre: no existe un archivo con el nombre del enlace.
    int value =  link(target, linkPath);
    if (value == -1) {
        perror("");
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

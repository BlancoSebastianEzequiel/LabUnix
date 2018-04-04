#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "general.h"
//------------------------------------------------------------------------------
// TEST EMPTY FILE
//------------------------------------------------------------------------------
void testEmptyFile(const char* ok, const char* error) {
    int fd = open("tmp", O_RDONLY);
    if (fd == -1) perr("ERROR: tmp file failed at function open");
    struct stat st;
    stat("tmp", &st);
    size_t size = (size_t) st.st_size;
    if (size == 0) printf("%s\n", ok);
    else printf("%s\n", error);
    if (close(fd) == -1) perr("ERROR: tmp file failed at function close");
    system("rm tmp");
}
//------------------------------------------------------------------------------
// RM0 TEST
//------------------------------------------------------------------------------
void rm0Test() {
    printf("testing rm0...");
    system("mkdir directory/");
    system("touch directory/model.stdout");
    system("touch directory/test.stdout");
    system("ls directory/ >> model.stdout");

    system("touch directory/archive.txt");
    system("./rm0 directory/archive.txt");
    system("ls directory/ >> test.stdout");

    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");
    system("rm -R directory/");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// CAT0 TEST
//------------------------------------------------------------------------------
void cat0Test() {
    printf("testing cat0...");
    system("cat longText.txt >> model.stdout");
    system("./cat0 longText.txt >> test.stdout");
    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// TOUCH0 TEST
//------------------------------------------------------------------------------
void touch0Test() {
    printf("testing touch0...");
    system("mkdir directory/");
    system("touch directory/model.stdout");
    system("touch directory/test.stdout");

    system("touch directory/archive.txt");
    system("ls directory/ >> model.stdout");
    system("rm directory/archive.txt");

    system("./touch0 directory/archive.txt");
    system("ls directory/ >> test.stdout");
    system("rm directory/archive.txt");

    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");
    system("rm -R directory/");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// STAT0 TEST
//------------------------------------------------------------------------------
void stat0Test() {
    printf("testing stat0...");

    system("touch archive.txt");
    system("echo 'Size: 0' >> model.stdout");
    system("echo 'File: archive.txt' >> model.stdout");
    system("echo 'Type: regular file' >> model.stdout");
    system("./stat0 archive.txt >> test.stdout");

    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");
    system("rm archive.txt");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// RM1 TEST
//------------------------------------------------------------------------------
void rm1Test() {
    printf("testing rm1...");
    system("mkdir directory/");
    system("touch directory/model.stdout");
    system("touch directory/test.stdout");
    system("ls directory/ >> model.stdout");

    system("touch directory/archive.txt");
    system("./rm1 directory/archive.txt");
    system("ls directory/ >> test.stdout");

    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");
    system("rm -R directory/");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// LN0 TEST
//------------------------------------------------------------------------------
void ln0Test() {
    printf("testing ln0...");

    system("touch archive.txt");

    system("ls -al >> model.stdout");
    system("./ln0 archive.txt enlace");
    system("ls -al >> test.stdout");
    system("diff model.stdout test.stdout >> tmp;");

    system("rm model.stdout");
    system("rm test.stdout");
    system("rm enlace");
    system("rm archive.txt");

    testEmptyFile("ERROR", "OK");
}
//------------------------------------------------------------------------------
// MV0 TEST
//------------------------------------------------------------------------------
void mv0Test() {
    printf("testing mv0...");

    system("touch archive.txt");

    system("mkdir directory/");

    system("mv archive.txt directory/archive.txt");
    system("ls directory/ >> model.stdout");
    system("rm directory/archive.txt");

    system("touch archive.txt");
    system("./mv0 archive.txt directory/archive.txt");
    system("ls directory/ >> test.stdout");
    system("rm directory/archive.txt");

    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");
    system("rm -R directory/");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// CP0 TEST
//------------------------------------------------------------------------------
void cp0Test() {
    printf("testing cp0...");

    system("echo 'Sistemas Operativos, 1er cuatrimestre 2018' >> model.stdout");
    system("./cp0 model.stdout test.stdout");
    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// TOUCH1 TEST
//------------------------------------------------------------------------------
void touch1Test() {
    printf("testing touch1...");

    system("echo 'Sistemas Operativos, 1er cuatrimestre 2018' >> archive.txt");
    system("stat archive.txt >> model.stdout");

    for (size_t i = 0; i < 1000000; i++) {}
    system("./touch1 archive.txt");
    system("stat archive.txt >> test.stdout");

    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");
    system("rm archive.txt");

    testEmptyFile("ERROR", "OK");
}
//------------------------------------------------------------------------------
// LN1 TEST
//------------------------------------------------------------------------------
void ln1Test() {
    printf("testing ln1...");

    system("echo 'Sistemas Operativos, 1er cuatrimestre 2018' >> archive.txt");
    system("ls -al >> model.stdout");
    system("./ln1 archive.txt enlace");
    system("ls -al >> test.stdout");
    system("diff model.stdout test.stdout >> tmp;");

    system("rm model.stdout");
    system("rm test.stdout");
    system("rm enlace");
    system("rm archive.txt");

    testEmptyFile("ERROR", "OK");
}
//------------------------------------------------------------------------------
// TEE0 TEST
//------------------------------------------------------------------------------
void tee0Test() {
    printf("testing tee0...");

    system("echo 'Hola' | tee dump.txt >> model.stdout");
    system("cat dump.txt >> test.stdout");
    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");
    system("rm dump.txt");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// LS0 TEST
//------------------------------------------------------------------------------
void ls0Test() {
    printf("testing ls0...");

    system("echo 'Sistemas Operativos' >> archive.txt");
    system("ls >> model.stdout");
    system("./ls0 >> test.stdout");

    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");
    system("rm archive.txt");

    testEmptyFile("ERROR", "OK");
}
//------------------------------------------------------------------------------
// CP1 TEST
//------------------------------------------------------------------------------
void cp1Test() {
    printf("testing cp1...");

    system("echo 'Sistemas Operativos, 1er cuatrimestre 2018' >> model.stdout");
    system("./cp1 model.stdout test.stdout");
    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// PS0 TEST
//------------------------------------------------------------------------------
void ps0Test() {
    printf("testing ps0...");

    system("ps -eo pid,comm >> model.stdout");
    system("./ps0 -eo pid,comm >> test.stdout");
    system("diff model.stdout test.stdout >> tmp;");
    system("rm model.stdout");
    system("rm test.stdout");

    testEmptyFile("OK", "ERROR");
}
//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    // Parte 1
    rm0Test();
    cat0Test();
    touch0Test();
    stat0Test();
    rm1Test();
    // Parte 2
    ln0Test();
    mv0Test();
    cp0Test();
    touch1Test();
    ln1Test();
    // Parte 3
    tee0Test();
    ls0Test();
    cp1Test();
    ps0Test();
    return 0;
}
//------------------------------------------------------------------------------

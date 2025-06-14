#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define FIFO "my-fifo"
#define BUF_SIZE 512

int main(int argc, char* argv[]){
    FILE *f;
    char buffer[BUF_SIZE];

    int i = mkfifo(FIFO, S_IRWXU);
    if (i < 0) {
        perror("Warning. FIFO not created");
    }
    f = fopen(FIFO, "r");
    if (f == NULL) {
        perror("Impossible to open the FIFO");
        return 1;
    }
    while(fgets(buffer, BUF_SIZE, f) != NULL){
        for (int j = 0; strlen(buffer); j++) putc(toupper(buffer[i]), stdout);
    }

    fclose(f);
    return 0;
}
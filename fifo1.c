#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define FIFO "my-fifo"
#define BUF_SIZE 512

int main(int argc, char * argv[]){
    FILE * f;
    char buffer[BUF_SIZE];
    if (argc != 2 || ( strcmp(argv [1], "read")==0 && strcmp(argv [1], "write")==0 ) ){
        printf("Usage: fifo read|write\n");
        return 1;
    }
    if (mkfifo(FIFO, S_IRWXU)<0) perror("Warning. FIFO not created");
    if (strcmp(argv [1], "read")==0){
        f = fopen(FIFO, "r");
    if (f==NULL){
        perror("Impossible to open the FIFO");
        return 1;
    }
        printf("Read mode:\n");
        while(fgets(buffer,BUF_SIZE,f)!=NULL) printf("%s", buffer);
    }else{
        f = fopen(FIFO, "w");
        if (f==NULL){
            perror("Impossible to open the FIFO");
            return 1;
    }
        printf("Write mode. Write lines of text:\n");
        while(fgets(buffer, BUF_SIZE, stdin)!=NULL){
            fputs(buffer, f);
            fflush(f);
        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
    char filename[100];
    if(argc < 2){
        printf("Uso: %s <nome_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        printf("Impossibile aprire il file. Errore: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else{
        char buffer[100];
        while(fgets(buffer, 100, file)){
            printf("%s", buffer);
        }
        printf("\n");
        fclose(file);
    }
    return 0;
}


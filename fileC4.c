#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
    if(argc<3){
        printf("Uso: %s <nome_file> <numero>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *fin = argv[1];
    char *fout= argv[2];
    
    FILE *file = fopen(fin, "r");
    if(file == NULL){
        printf("Impossibile aprire il file %s. Errore: %s\n", fin, strerror(errno));
        return EXIT_FAILURE;
    }

    FILE *fil = fopen(fout, "w");
    if(fil == NULL){
        printf("Impossibile aprire il file %s. Errore: %s\n", fout, strerror(errno));
        fclose(file);
        return EXIT_FAILURE;
    }
    int num, min;
    fscanf(file, "%d", &min);
    while(fscanf(fil, "%d\n", &num) != EOF){
        if(num < min){
            min = num;
        }
    }
    fprintf(fil, "Il numero minimo nel file %s è: %d\n", fin, min);
    fclose(fil);
    fclose(file);
    return 0;
}
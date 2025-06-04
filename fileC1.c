#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(){
    char buffer[100];
    FILE * file = fopen("file.txt", "r");
    if (file == NULL) {
        printf("Impossibile aprire il file. Errore: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    else{
        printf("File opened successfully.\n");
        while(fgets(buffer, 100, file)){
            printf("%s", buffer);
        }
        printf("\n");
        fclose(file);
        printf("File closed successfully.\n");
    }

    int n;
    printf("Inserisci un numero: ");
    scanf("%d", &n);
    file = fopen("file.txt", "w");
    if (file == NULL){
        printf("Impossibile aprire il file. Errore: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    for (int i=1; i<=n; i++) fprintf(file, "%d\n", i);
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int factor(int n) {
    if(n == 0 || n == 1) return 1;
    return n * factor(n - 1);
}

int max(int *array, int dim) {
    int max_value = array[0];
    for(int i = 1; i < dim; i++) {
        if(array[i] > max_value) {
            max_value = array[i];
        }
    }
    return max_value;
}

int max_pos(int *array, int dim) {
    int max_value = array[0];
    int pos = 0;
    for(int i = 1; i < dim; i++) {
        if(array[i] > max_value) {
            max_value = array[i];
            pos = i;
        }
    }
    return pos;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }

    int dim = 0;
    char buffer[1000];
    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        dim++;
    }
    rewind(file);

    int *number = malloc(dim * sizeof(int));
    if(number == NULL) {
        perror("Errore nell'allocazione della memoria");
        fclose(file);
        return 1;
    }

    for(int i = 0; i < dim; i++) {
        pid_t pid = fork();
        if(pid == -1) {
            perror("Errore nella fork");
            free(number);
            fclose(file);
            return 1;
        }
        
        if(pid == 0) { 
            if(fgets(buffer, sizeof(buffer), file) == NULL) {
                perror("Errore nella lettura del file");
                exit(1);
            }
            int val = atoi(buffer);
            exit(factor(val));
        }
    }

    for(int i = 0; i < dim; i++) {
        int status;
        wait(&status);
        if(WIFEXITED(status)) {
            number[i] = WEXITSTATUS(status);
        } else {
            number[i] = -1; 
        }
    }

    printf("Il valore massimo è %d alla riga %d\n", 
           max(number, dim), max_pos(number, dim) + 1);

    free(number);
    fclose(file);
    return 0;
}
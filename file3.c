#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

static int dimensione = 0;
int *ptr;

void function(char* buffer){
    char token[1024];
    int dim = 0;
    FILE *file = fopen(buffer, "r");
    while(fgets(file, token, sizeo(token)) != NULL){
        dim += atoi(token);
    }
    dimensione += 1;
    ptr = realloc(ptr, dimensione * sizeof(int));
    ptr[dimensione - 1] = dim;
}


int main(int argc, char* argv[]){
    char buffer[1024];
    pthread_t thread;
    void* res;

    FILE *file = fopen(argv[1], "r");
    while(fgets(file, buffer, sizeof(buffer) != NULL)){
        int s = pthread_create(&thread, NULL, function, buffer);
    }
    pthread_join(thread, &res);
    fclose(file);

    int sum = 0;
    for(int i = 0; i < dimensione; i++){
        sum += ptr[i];
    }
    printf("Sum: %d\n", sum);
    return 0;
}
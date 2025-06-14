//si crei un programma che genera un figlio. Il processo padre riceve una stringa da riga
//di comando e la passa al figlio tramite una pipe. Il figlio riceve la stringa e la stampa.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    if(fork() > 0){
        close(pipefd[0]);
        write(pipefd[1], argv[1], strlen(argv[1]) + 1); 
        close(pipefd[1]);
    } else{
        close(pipefd[1]);
        read(pipefd[0], argv[1], 100);
        printf("Ricevuto dal figlio: %s\n", argv[1]);
        close(pipefd[0]);
    }
    return 0;
}
//si crei un programma con due processi. Il processo padre riceve il nome di un file da riga di comando 
//e ne passa il contenuto al figlio tramite una pipe. Il figlio riceve il contenuto e lo stampa.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    if(fork() > 0){
        close(pipefd[0]);
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            perror("open");
            close(pipefd[1]);
            return 1;
        }
        char buffer[100];
        while(read(fd, buffer, sizeof(buffer)) >0){
            write(pipefd[1], buffer, strlen(buffer));
        }
        close(fd);
    }
    else{
        close(pipefd[1]);
        char buffer[100];
        while(read(pipefd[0], buffer, sizeof(buffer))>0){
            printf("Ricevuto dal padre: %s", buffer);
        }
        close(pipefd[0]);
    }
    return 0;
}
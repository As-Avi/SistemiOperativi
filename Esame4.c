#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>

void function(int signum){
    printf(" Digita Y se vuoi terminare il programma o N se vuoi continuare.\n");
    char buffer[2];
    while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if(toupper(buffer[0]) == 'Y') {
            printf("Programma terminato.\n");
            exit(0);
        } else if(toupper(buffer[0]) == 'N') {
            printf("Continuo l'esecuzione del programma.\n");
           return;
        } else {
            printf("Input non valido. Digita Y per terminare o N per continuare.\n");
        }
        while(getchar() != '\n');
    }
}


int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    if (pid <= 0) {
        fprintf(stderr, "Invalid PID: %s\n", argv[1]);
        return 1;
    }   

    // Controlla se il processo esiste
    if (kill(pid, 0) == -1) {
        if (errno == ESRCH) {
            fprintf(stderr, "Process with PID %d does not exist.\n", pid);
            return 1;
        } else if (errno == EPERM) {
            fprintf(stderr, "Process with PID %d exists but permission denied.\n", pid);
            return 1;
        } else {
            perror("Error checking process");
            return 1;
        }
    }   
    
    struct sigaction sa;
    sa.sa_handler = function;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    while(1){ 
        printf("Premi Ctrl+C per inviare un segnale al processo con PID %d.\n", pid);
        sleep(5);
        if (kill(pid, SIGKILL) == -1) {
            if (errno == ESRCH) {
                printf("Processo con PID %d terminato.\n", pid);
                return 0;
            }
            perror("kill");
            return 1;
        }
        
    }

    return 0;
}
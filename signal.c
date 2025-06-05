#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*si scriva un programma in C che memorizza quanti
SIGTERM ha ricevuto. Alla pressione di CTRL+C stampa tale
numero e termina. Si nomini il programma sample */

int sigterm_count = 0;

void handler(int signum){
    if(signum == SIGTERM){
        sigterm_count++;
    } else if (signum == SIGINT) {
        printf("Numero di SIGTERM ricevuti: %d\n", sigterm_count);
        exit(0); // Termina il programma
    }

}

int main(int argc, int *argv[]){
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;
    sigaction (SIGTERM, &action, NULL);
    sigaction (SIGINT, &action, NULL);
    while (1);
    return 0;
}

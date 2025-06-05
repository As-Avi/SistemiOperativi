#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 128

int main() {
    char buf[MAXLINE];
    pid_t pid;
    int status;
    printf("%% "); /* prompt */

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = 0;
        if ((pid = fork()) < 0){ 
            printf("errore di fork "); 
            exit(1);
        }
        else if (pid == 0) { /*figlio */
        execlp(buf, buf, NULL);
        printf("non posso eseguire: %s\n", buf);
        exit(127);
        } else 
        if ((pid = waitpid(pid, &status, 0)) < 0){ /* padre */
        printf("errore di waitpid"); 
        exit(1);
        }
    printf("%% ");
    }

exit(0);
}
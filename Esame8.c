#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "r");
    char buffer[256];
    pid_t pid[256];
    int i = 0;
    while(fgets(buffer, sizeof(buffer), file) != NULL){
        /* buffer[strcspn(buffer, "\n")] = 0;
        pid[i] = fork();
        if(pid[i] == 0){
            execlp("/bin/sh", "sh", "-c", buffer ,(char *)NULL); 
            perror("execvp failed");
            return(1);
        }
        i += 1;
    }
    for(int j = 0; j++; j< i) waitpid(pid[j], NULL, 0); */
    if(!system(buffer)) return 1;
    }
fclose(file);
return 0;
}
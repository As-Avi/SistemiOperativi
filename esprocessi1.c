#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    __pid_t pid;
    printf("T1\n");
    pid = fork();

    if(pid==0){
        printf("T3\n");
        return 0;
    }
    else{
        printf("T2\n");wait(NULL);
        wait(NULL);
        wait((int *) 0);
    }
    printf("T4\n");
    return 0;
}
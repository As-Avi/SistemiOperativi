#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    __pid_t pid;
    printf("start\n");
    pid = fork();

    if(pid==0){
        printf("b.com\n");
        pid = fork();
        if(pid==0){
            printf("https://www.b.com/2\n");
            return 0;
        }
        else{
            printf("https://www.b.com/1\n");
            wait(NULL);
            return 0;
        }
        return 0;
    }
    else{
        printf("a.com\n");
        printf("https://www.a.com/\n");
        return 0;
    }
    printf("end\n");
    return 0;
}
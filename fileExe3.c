#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void Figlio(void);
void Padre(void);

int main(){
    pid_t pid;
    pid = fork();
    if (pid == 0) Figlio();
    else Padre();
}

void Figlio(void){
    int i=0;
    for(i=0;i<10;i++){
        usleep(200);
        printf("\tSono il figlio. i= %d\n",i);
    }
}

void Padre(void){
    int i=1;
    for(i=0;i<10;i++){
        usleep(250);
        printf("Sono il padre. i= %d\n",i);
    }
}
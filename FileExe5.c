/*Write a C program that has two processes. The first process sends to the second the name of a file. The second process reads the name of the file and sends back to the first process its content.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void client(int read_fd, int write_fd){
    char buffer[100];

    printf("Enter the name of the file: ");
    scanf("%s", buffer);

    write(write_fd, buffer, strlen(buffer) + 1);
    while (read(read_fd,buffer,sizeof(buffer)) > 0) printf("%s",buffer);
}   

void server(int read_fd, int write_fd){
    FILE * fp;
    char buffer[1000];
    int n;
    n = read(read_fd,buffer,sizeof(buffer));
    buffer[n]= 0;

    fp=fopen(buffer,"r");
    while(fgets(buffer,sizeof(buffer),fp)!=NULL) write(write_fd,buffer,sizeof(buffer));
}

int main(int argc, char *argv[]){
    int pfd1[2], pfd2[2], pid, status;
    pipe(pfd1);
    pipe(pfd2);

    pid = fork();
    if(pid > 0){
        close(pfd1[0]); close(pfd2[1]);
        client(pfd2[0], pfd1[1]);
        wait(&status);
        exit(0);
    } else {
        close(pfd1[1]); close(pfd2[0]);
        server(pfd1[0], pfd2[1]);
        exit(0);
    }

    return 0;
}


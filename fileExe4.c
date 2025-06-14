/*Write a C program that forks a child, which forks a child. Every second, the
grand-child sends a SIGUSR1 to the the child (which is the father of the grand-
child). When the child receives a SIGUSR1, it sends a SIGUSR2 to the father.*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void handler_child(int signum){
printf("Child received SIGUSR1 and sends SIGUSR2\n");
kill (getppid(), SIGUSR2);
}

void handler_father(int signum){
printf("Father received SIGUSR2\n");
}

int main(){
    pid_t pid;
    struct sigaction action;
    pid = fork();
    if (pid > 0){
        action.sa_handler = handler_father;
        sigemptyset (&action.sa_mask);
        action.sa_flags = 0;
        sigaction (SIGUSR2, &action, NULL);
    while (1);
    } else {
        if(pid > 0){
            action.sa_handler = handler_child;
    sigemptyset (&action.sa_mask);
    action.sa_flags = 0;
    sigaction (SIGUSR1, &action, NULL);
    while (1);
        } else {
            while (1){
                sleep(1);
                kill (getppid(), SIGUSR1);
            }
        }
    }
    return 0;
}
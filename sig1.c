#include <signal.h>
#include <stdio.h>
//pkill -KILL <nome prog>

void func(int signum){
printf("ricevo %d\n", signum);
}

int main (int argc, char *argv[]){
    struct sigaction new_action, old_action;
    new_action.sa_handler = func;
    sigemptyset ( &new_action.sa_mask); /* Si noti l'uso di sigemptyset */
    new_action.sa_flags = 0;
    sigaction (SIGINT, &new_action, NULL);
    sigaction (SIGTERM, &new_action, NULL);
    while(1) ;
    return 0;
}
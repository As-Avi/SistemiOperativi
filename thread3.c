#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void * func(void *arg){
    sleep(5);
    //exit(0); ci metterebbe 5 secondi a terminare il thread
}

int main(){
    pthread_t t;
    int ret;
    pthread_create(&t, NULL, func, NULL);
    sleep (10);
    pthread_join(t, NULL);
    exit(0); //ci mette 10 secondi a terminare il thread
}
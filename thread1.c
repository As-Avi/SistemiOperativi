#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//creazione di un thread che stampa un messaggio e ritorna la lunghezza del messaggio

static void * threadFunc(void *arg){
    printf("From Thread: %s", (char *) arg);
    int * ret = malloc(sizeof(int));
    *ret = strlen(arg);
    return ret ; // Valore di ritorno del thread, equivale a pthread_exit(ret);
}

int main(int argc, char *argv[]){
    pthread_t t1;
    void *res; // Per valore di ritorno
    int s;
    s = pthread_create(&t1, NULL, threadFunc, "Hello world\n"); // Creazione
    if (s != 0){
        printf("Cannot create thread");
        exit(1);
    }
    printf("Message from main()\n");
    s = pthread_join(t1, &res); // Join. Richiede un void **, ovvero &res
    if (s != 0){
        printf("Cannot join thread");
        exit(1);
    }
    printf("Thread returned %d\n", *((int *)res) ); // Utilizzo del valore di ritorno
    free (res); // Needed as that zone was allocated with malloc
    exit(0);
}
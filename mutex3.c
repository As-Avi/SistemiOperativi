#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <string.h>
#include <sys/stat.h>  
//stampa una stringa ogni volta che mutex2 effettua una post su un semaforo

int main(int argc, char *argv[]){
    sem_t * s;
    int i = 0;

    s = sem_open("/semaforo", O_CREAT , S_IRUSR | S_IWUSR, 0);
    if(s == SEM_FAILED) {
        printf("Error creating/opening the semaphore %s\n", strerror(errno));
        exit (1);
    }
    while(1){
        sem_wait(s);
        printf("Post %d effettuata da mutex2\n", i);
        i++;
    }
    sem_close(s); /* Codice irraggiungibile*/
    return 0;
}
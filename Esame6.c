#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>

static int glob = 0;
static pthread_mutex_t mtx;
static void *function(void *arg){
    int number = *((int*) arg);
    for(int i = 0; i < number; i++){
        pthread_mutex_lock(&mtx);
        glob++;
        pthread_mutex_unlock(&mtx);
    }
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }
    pthread_t thread1, thread2, thread3;
    int number = atoi(argv[1]);
    if(pthread_mutex_init(&mtx, NULL) != 0){
        fprintf(stderr, "Error initializing mutex: %s\n", strerror(errno));
        return 1;
    }
    pthread_create(&thread1, NULL, function, &number);
    pthread_create(&thread2, NULL, function, &number);
    pthread_create(&thread3, NULL, function, &number);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_mutex_destroy(&mtx);
    printf("glob = %d\n", glob);

    return 0;
}

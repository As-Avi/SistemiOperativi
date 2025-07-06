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

static void* function(void *arg){
    char *filename = (char *) arg;
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        fprintf(stderr, "Error opening file %s: %s\n", (char*) arg, strerror(errno));
        pthread_exit(NULL); 
    }
    char line[256];
    int *count = malloc(sizeof(int));
    *count = 0;
    while(fgets(line, sizeof(line), file) != NULL) *count += atoi(line);
    return count;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }
    FILE * file = fopen(argv[1], "r");
    if(file == NULL){
        fprintf(stderr, "Error opening file %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    char line[256];
    int total_count = 0;
    pthread_t threads[100]; 
    int thread_count = 0;
    void *ret_vals[100];

    while (fgets(line, sizeof(line), file) != NULL && thread_count < 100) {
        int s = pthread_create(&threads[thread_count], NULL, function, line);
        if (s != 0) {
            fprintf(stderr, "Error creating thread: %s\n", strerror(s));
            continue;
        }
        thread_count++;
    }

    for (int i = 0; i < thread_count; i++) {
        void *ret;
        pthread_join(threads[i], &ret);
        
        if (ret != NULL) {
            total_count += *(int *)ret;
            free(ret);  
        }
    }

    printf("Total count: %d\n", total_count);
    fclose(file);
    return 0;
}
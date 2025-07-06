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
#include <semaphore.h>

int main(int argc, char *argv){
    sem_t * s = sem_open("/semafoto", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (s == SEM_FAILED) {
        fprintf(stderr, "Error creating/opening the semaphore: %s\n", strerror(errno));
        exit(1);
    }
    FILE *file = fopen("myfile.txt", "wr+");
    pid_t pid = fork();
    if(pid == 0){
        sem_wait(s);
        char line[256];
        while(fgets(line, sizeof(line), file) != NULL) {
            printf("C%s", line);
        }
    }
    else{
        sem_post(s);
        while(1){
            getchar();
            printf("Scrivi n numeri, digita una lettera per terminare\n");
            char x = getchar();
            if(!isdigit(x)) break;
            else fprintf(file, "%c\n", x);
        }
        wait(NULL);
    }
    fclose(file);
    sem_close(s);
    return 0;
}
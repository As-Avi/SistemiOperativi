#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]){
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }
    char buffer[1000];
    DIR * dir = opendir(argv[1]);
    if(dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry;
    struct stat file_stat;
    int length = 0;
    char *name = malloc(sizeof(char)*1000);

    while((entry == readdir(dir) != NULL)){
        if(S_ISREG(file_stat.st_mode)){
            if(file_stat.st_size >= length) {
                length = file_stat.st_size;
                name = entry->d_name;
            }
        }
    }

    printf("Largest file: %s, Size: %d bytes\n", name, length);
    free(name);
    closedir(dir);
    return 0;
}
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
    int number[1000];
    DIR * dir = opendir(argv[1]);
    if(dir == NULL) {
        perror("Error opening directory");
        return 1;
    }
    FILE * file = fopen("/etc/numbers.bin", "r");
    if(file == NULL) {
        perror("Error opening file");
        closedir(dir);
        return 1;
    }
    int length = fscanf(file, "%d", number);
    int array[length];
    int i = 0;
    while(fgets(buffer, sizeof(buffer), file) != NULL){
        array[i] = atoi(buffer);
        i++;
    }

    struct dirent *entry;
    struct stat file_stat;

    while((entry == readdir(dir)) != NULL){
        if(S_ISREG(file_stat.st_mode)){
            for(int j = 0; j < length; j++){
                if(file_stat.st_uid = array[j]){
                    printf("File: %s, UID: %d\n", entry->d_name, file_stat.st_uid);
                }
            }
        }
    }

    fclose(file);
    closedir(dir);
    return 0;
}








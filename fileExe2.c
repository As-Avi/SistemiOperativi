#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#define PATH_MAX 4096

void listdir(const char *dir_name){
    DIR *folder = opendir(dir_name);
    if(folder == NULL){
        perror("Unable to read directory");
        exit(EXIT_FAILURE);
    }
    while(1){
        struct dirent *entry = readdir(folder);
        const char *d_name;
        if(entry == NULL) break;
        d_name = entry->d_name;
        printf("%s/%s\n", dir_name, d_name);
        if(entry -> d_type == DT_DIR){
            if(strcmp(d_name, ".") != 0 && strcmp(d_name, "..") != 0){
                char path[PATH_MAX];
                int path_lenght = snprintf(path, sizeof(path), "%s/%s", dir_name, d_name);
                if(path_lenght >= sizeof(path)){
                    fprintf(stderr, "Path too long: %s/%s\n", dir_name, d_name);
                }
                listdir(path);
            }
        }
    }
    if (closedir(folder)) {
            fprintf(stderr, "Could not close '%s': %s\n", dir_name, strerror(errno));
            exit(EXIT_FAILURE);
        }
}


int main(int argc, char *argv[]){
    if(argc<2){
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }
    listdir(argv[1]);

    return 0;
}
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    DIR *dir = opendir(".");
    struct dirent *entry;
    struct stat file_stat;

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt") != NULL) {
            stat(entry->d_name, &file_stat);
            printf("File: %s - Dimensione: %ld bytes\n", entry->d_name, file_stat.st_size);
        }
    }
    closedir(dir);
    return 0;
}
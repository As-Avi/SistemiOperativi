#include <stdio.h>
#include <dirent.h>

int main(){
    DIR *folder;
    struct dirent *entry;

    // Open the current directory
    folder = opendir(".");
    if(folder==NULL){
        perror("Unable to read directory");
        return 1;
    }
    // Read and print the names of files in the directory
    while((entry = readdir(folder)) != NULL){
        printf("%s\n", entry->d_name);
    }

    closedir(folder);
    return 0;
}

#include <stdio.h>
#include <stdlib.h> 
#include <sys/stat.h> 
#include <dirent.h> 
#include <pwd.h>
#include <grp.h>
#include <string.h>
#define PATH_MAX 4096

int main(int argc, char *argv[]){
    struct stat buf;
    char path[PATH_MAX];

    if (argc!=2){
        printf("Specifica un path\n");
        return 1;
    }

    if (stat(argv[1], &buf) < 0) {
        printf ("Impossibile leggere le informazioni sulla directory\n");
        exit (1); 
    }

    if(!S_ISDIR(buf.st_mode)) {
        printf("%s: non è una directory\n", argv[1]);
        return 1; 
    }
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("Impossibile aprire la directory");
        return 1; 
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", argv[1], entry->d_name);
        if (lstat(path, &buf) < 0) {
            printf("Impossibile leggere le informazioni sul file");
            continue; 
        }

        printf("Node: %s\n", path);
        printf("Inode: %lu\n", buf.st_ino);

        if (S_ISREG(buf.st_mode)) {
            printf("Type: file\n");
        } else if (S_ISDIR(buf.st_mode)) {
            printf("Type: directory\n");
        } else if (S_ISLNK(buf.st_mode)) {
            printf("Type: symbolic link\n");
        } else if (S_ISFIFO(buf.st_mode)) {
            printf("Type: FIFO\n");
        } else if (S_ISCHR(buf.st_mode)) {
            printf("Type: character device\n");
        } else if (S_ISBLK(buf.st_mode)) {
            printf("Type: block device\n");
        } else if (S_ISSOCK(buf.st_mode)) {
            printf("Type: socket\n");
        } else {
            printf("Type: other\n");
        }

        printf("Size: %ld\n", buf.st_size);
        printf("Owner: %d %s\n", buf.st_uid, getpwuid(buf.st_uid)->pw_name);
        printf("Group: %d %s\n", buf.st_gid, getgrgid(buf.st_gid)->gr_name);
        printf("\n");
        
    }
    int r = closedir(dir);
    if (r < 0) {
        perror("Impossibile chiudere la directory");
        return 1; 
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <unistd.h>
#define PATH_MAX 4096

void print_file_info(const char *path, const char *base_path) {
    struct stat stat_buf;
    if (lstat(path, &stat_buf) == -1) {
        perror("lstat");
    }

    const char *type;
    if (S_ISREG(stat_buf.st_mode)) type = "file";
    else if (S_ISDIR(stat_buf.st_mode)) type = "directory";
    else if (S_ISLNK(stat_buf.st_mode)) type = "symbolic link";
    else if (S_ISFIFO(stat_buf.st_mode)) type = "FIFO";
    else type = "other";

    struct passwd *pw = getpwuid(stat_buf.st_uid);
    struct group *gr = getgrgid(stat_buf.st_gid);

    printf("Node: %s\n", path);
    printf("Inode: %lu\n", stat_buf.st_ino);
    printf("Type: %s\n", type);
    printf("Size: %ld\n", stat_buf.st_size);
    printf("Owner: %d %s\n", stat_buf.st_uid, pw ? pw->pw_name : "unknown");
    printf("Group: %d %s\n", stat_buf.st_gid, gr ? gr->gr_name : "unknown");
    printf("\n");
}

void scan_directory(const char *path, const char *base_path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        char rel_path[PATH_MAX];
        if (strcmp(base_path, ".") == 0) {
            snprintf(rel_path, sizeof(rel_path), "./%s", entry->d_name);
        } else {
            snprintf(rel_path, sizeof(rel_path), "%s/%s", base_path, entry->d_name);
        }

        print_file_info(rel_path, base_path);

        struct stat stat_buf;
        if (lstat(full_path, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode)) {
            scan_directory(full_path, rel_path);
        }
    }

    closedir(dir);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    const char *base_path = argv[1];
    print_file_info(base_path, base_path);
    scan_directory(base_path, base_path);

    return 0;
}
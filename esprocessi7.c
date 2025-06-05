#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


int system(const char *cmd){
    int stat;
    pid_t pid;
    if (cmd == NULL) return(1);
    if ((pid = fork()) == 0) { /* Son */
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127);
    }
    if (pid == -1) {
        stat = -1; /* Error */
    } else { /* Father */
        while (waitpid(pid, &stat, 0) == -1) {
        if (errno != EINTR){
            stat = -1;
            break;
            }
        }
    }
    return(stat);
}

int main(int argc, char *argv[]) {
    char command[50] = "ls -lh ";
    if (argc == 2) {
        strncat(command, argv[1], sizeof(command) - strlen(command) - 1);
    }
    int result = system(command);
    if (result == -1) {
        perror("system");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
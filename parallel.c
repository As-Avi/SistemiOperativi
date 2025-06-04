#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXPROCESSES 256
#define MAXCMD 1024
#define MAXLINE 1024

void replace_percent(char *command, const char *param) {
    char *percent_pos = strstr(command, "%");
    if (percent_pos == NULL) {
        return; 
    }
    
    size_t prefix_len = percent_pos - command;
    size_t suffix_len = strlen(percent_pos + 1);
    size_t param_len = strlen(param);

    if (prefix_len + param_len + suffix_len >= MAXCMD) {
        fprintf(stderr, "Command too long after substitution\n");
        exit(EXIT_FAILURE);
    }
    
    char temp[MAXCMD];
    strncpy(temp, command, prefix_len);
    temp[prefix_len] = '\0';
    strcat(temp, param);
    strcat(temp, percent_pos + 1);
    strcpy(command, temp);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <file> <num_processes> <command>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    const char *filename = argv[1];
    int num_processes = atoi(argv[2]);
    char *command_template = argv[3];
    
    if (num_processes <= 0 || num_processes > MAXPROCESSES) {
        fprintf(stderr, "Invalid number of processes (must be 1-%d)\n", MAXPROCESSES);
        return EXIT_FAILURE;
    }
    
    if (strstr(command_template, "%") == NULL) {
        fprintf(stderr, "Command template must contain %% character\n");
        return EXIT_FAILURE;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    
    char params[MAXPROCESSES][MAXLINE];
    int num_params = 0;
    
    while (fgets(params[num_params], MAXLINE, file) != NULL && num_params < MAXPROCESSES) {
        size_t len = strlen(params[num_params]);
        if (len > 0 && params[num_params][len-1] == '\n') {
            params[num_params][len-1] = '\0';
        }
        num_params++;
    }
    fclose(file);
    
    if (num_params == 0) {
        fprintf(stderr, "No parameters found in file\n");
        return EXIT_FAILURE;
    }
    
    int pipes[MAXPROCESSES][2];
    for (int i = 0; i < num_processes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return EXIT_FAILURE;
        }
    }
    
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return EXIT_FAILURE;
        }
        
        if (pid == 0) { 
            for (int j = 0; j < num_processes; j++) {
                close(pipes[j][1]);
            }
                        
            char cmd[MAXCMD];
            ssize_t bytes_read;
            
            while ((bytes_read = read(pipes[i][0], cmd, MAXCMD)) > 0) {
                system(cmd);
            }
            
            close(pipes[i][0]);
            exit(EXIT_SUCCESS);
        }
    }
    
    for (int i = 0; i < num_processes; i++) {
        close(pipes[i][0]);
    }
    
    for (int i = 0; i < num_params; i++) {
        char cmd[MAXCMD];
        strcpy(cmd, command_template);
        replace_percent(cmd, params[i]);
        
        int child_idx = i % num_processes;
        if (write(pipes[child_idx][1], cmd, MAXCMD) == -1) {
            perror("write");
            return EXIT_FAILURE;
        }
    }
    
    for (int i = 0; i < num_processes; i++) {
        close(pipes[i][1]);
    }
    
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }
    
    return EXIT_SUCCESS;
}
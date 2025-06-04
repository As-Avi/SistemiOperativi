#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main (int argc, char ** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    char str[12]; 
    int n = atoi(argv[1]);

    if (n <= 0) {
        printf("End!\n");
        return 0;
    }

    printf("%d\n", n - 1);
    snprintf(str, sizeof(str), "%d", n - 1);

    if (execl(argv[0], argv[0], str, NULL) == -1) {
        fprintf(stderr, "Error executing execl: %s\n", strerror(errno));
        return 1;
    }

    return 0; 
}
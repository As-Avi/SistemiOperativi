#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char ** argv) {
    char str[10];
    int n;
    n = atoi(argv[1]) - 1;
    printf ("%d\n", n);
    if (n>0) {
        sprintf (str, "%d", n);
        execl (argv[0], argv[0], str, (char*)NULL);
    }
    printf ("End!\n");
    return 0;
}
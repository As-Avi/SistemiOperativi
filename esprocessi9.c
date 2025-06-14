#include <stdio.h>
#include <unistd.h>
int main(){
    int N = 2;
    for (int i=0; i<N; i++)
    fork();
    printf("Ciao\n");
}
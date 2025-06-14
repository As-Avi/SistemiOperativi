/* Write a C program that has two processes and shared memory containing N floating point numbers. The child process fills the shared memory float vector,
where each element v[i]=10 * i, then it terminates. The father process waits for the child to terminate, then it prints the vector. */

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#define N 10

int main (void){
    int i;
    float* shared_memory;
    shared_memory=mmap(0, N*sizeof(float), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    
    if (fork()){
        wait(NULL);
        for(i=0; i<N; i++) printf("shared_memory[%d]==%f\n", i, shared_memory[i]);
    } else{
        for(i=0; i<N; i++) shared_memory[i]=i*10;
    }

    return 0;
}
#include <stdio.h>
#include <unistd.h>

int main(){
    if ( fork() ){
        printf("A\n");
    }else{
        fork();
        printf("B\n");
    }
}
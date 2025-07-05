#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char* argv[]){
    char buffer[1024];
    char array[1024];

    int file1 = open(argv[1], O_RDONLY);
    int file2 = open(argv[2], O_RDWR);
    if(file1 == 0) read(file1, buffer, sizeof(buffer));

    FILE *file1_ptr = fdopen(file1, "r");
    fseek(file1_ptr, 0, SEEK_END);
    long file_size = ftell(file1_ptr);
    fclose(file1_ptr);

    for(int i = 0; i < file_size; i++) array[i] = buffer[1024-i];
    if(file2 == 0) write(file2, array, sizeof(array));

    return 0;
}
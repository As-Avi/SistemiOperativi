#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SIZE 1024

int main(int argc, char *argv[]){
    struct sockaddr_in address;
    int n;
    char buffer[SIZE];
    
    //Creazione 
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // Connessione: specifica indirizzo IP e porta del server 
    connect(fd, (struct sockaddr*)&address, sizeof(address));
    
    // Input/Output
    write(fd, buffer, n);
    read(fd, buffer, SIZE);
    
    // Chiusura
    close(fd);
    return 0;
}



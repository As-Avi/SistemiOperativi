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
    int addrlen = sizeof(address);
    
    // Creazione
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // Bind: specifica porta
    bind(fd, (struct sockaddr*)&address, sizeof(address));
    // Listen: specifica lunghezza della coda in attesa
    listen(fd, 3);

    // Servizio ai client
    while (1){
        // Attesa di un client: ottiene indirizzo IP e porta del client 
        int active_fd = accept(fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        
        // Input/Output
        write(active_fd, buffer, n);
        read(active_fd, buffer, SIZE);
        
        // Chiusura
        close(active_fd);
    }

    // Chiusura
    close(fd);
    return 0;
}

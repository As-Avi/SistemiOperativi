#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

unsigned long long calcola_fattoriale(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    unsigned long long risultato = 1;
    for (int i = 2; i <= n; i++) {
        risultato *= i;
    }
    return risultato;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Utilizzo: %s <nome_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    }

    int numero;
    int riga = 0;
    int max_numero = -1;
    int max_riga = -1;
    int *numeri = NULL;
    int count = 0;

    // Legge i numeri dal file e trova il massimo
    while (fscanf(file, "%d", &numero) == 1) {
        riga++;
        if (numeri == NULL) {
            numeri = malloc(sizeof(int));
        } else {
            numeri = realloc(numeri, (count + 1) * sizeof(int));
        }
        numeri[count] = numero;
        count++;

        if (numero > max_numero) {
            max_numero = numero;
            max_riga = riga;
        }
    }
    fclose(file);

    // Crea un processo figlio per ogni numero
    for (int i = 0; i < count; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Processo figlio
            unsigned long long fattoriale = calcola_fattoriale(numeri[i]);
            printf("Fattoriale di %d: %llu\n", numeri[i], fattoriale);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("Errore nella fork");
            exit(EXIT_FAILURE);
        }
    }

    // Attende la terminazione di tutti i figli
    for (int i = 0; i < count; i++) {
        wait(NULL);
    }

    // Stampa il numero più grande e la sua riga
    printf("Numero più grande: %d (riga %d)\n", max_numero, max_riga);

    free(numeri);
    return 0;
}
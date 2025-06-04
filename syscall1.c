//Si scriva un programma che riceve un path da riga di comando e stampa se esso é file, directory o link simbolico.
#include <stdio.h>
#include <stdlib.h> // Necessario per exit
#include <sys/stat.h> // Necessario per stat

int main (int argc, char * argv[]){
    struct stat buf;
    if (argc!=2){
        printf("Specifica un path\n");
        return 1;
    }

    if (stat(argv[1], &buf) < 0) {
        printf ("Impossibile leggere le informazioni sul file\n");
        exit (1); /* Termina subito il programma con codice 1 */
    }

    if (S_ISREG(buf.st_mode)) printf("%s: file\n", argv[1]); //Controlla se è un file regolare
    else if (S_ISDIR(buf.st_mode)) printf("%s: directory\n", argv[1]); //Controlla se è una directory
    else if (S_ISLNK(buf.st_mode)) printf("%s: link simbolico\n", argv[1]); //Controlla se è un link simbolico
    else printf("%s: altro tipo di path\n", argv[1]);
    return 0;
}
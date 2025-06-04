#include <stdio.h>
#include <stdlib.h> // Necessario per exit
#include <sys/stat.h> // Necessario per stat
#include <dirent.h> // Necessario per struct dirent 

//Si scriva un programma che riceve una directory da riga di comando e ne lista il contenuto
int main(int argc, char *argv[]){
    struct stat buf;
    if (argc!=2){
        printf("Specifica un path\n");
        return 1;
    }

    if (stat(argv[1], &buf) < 0) {
        printf ("Impossibile leggere le informazioni sulla directory\n");
        exit (1); /* Termina subito il programma con codice 1 */
    }

    if(!S_ISDIR(buf.st_mode)) {
        printf("%s: non è una directory\n", argv[1]);
        return 1; // Esce se non è una directory
    }
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("Impossibile aprire la directory");
        return 1; // Esce se non riesce ad aprire la directory
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue; // Ignora le voci speciali "." e ".."
        printf("%s\n", entry->d_name);
    }
    int r = closedir(dir);
    if (r < 0) {
        perror("Impossibile chiudere la directory");
        return 1; // Esce se non riesce a chiudere la directory
    }
    printf("Elenco dei file nella directory %s completato.\n", argv[1]);

    return 0;
}


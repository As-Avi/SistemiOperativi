#!/bin/bash
while true ; do
    echo "Inserisci il nome del file (o 'quit' per uscire): "
    read filename
    if [[ $filename == "quit" ]]; then
        echo "Uscita dal programma."
        break
    fi
    if [[ -f "$filename" ]]; then
        echo "Il file '$filename' ha come numero di linee:"
        wc -l $filename
    elif [[ -d "$filename" ]]; then
        echo "Il file '$filename' è una directory."
    else
        echo "Errore: il file '$filename' non esiste."
    fi
done
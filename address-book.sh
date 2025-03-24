#!/bin/bash

CSV_FILE="address-book-database.csv"

if [[ ! -f "$CSV_FILE" ]]; then
    echo "Errore: Il file $CSV_FILE non esiste."
fi

case $1 in 
    view)
        cat "$CSV_FILE" | column -s, -t | head -l 
        tail -n $(wc -l "$CSV_FILE") | sort -t, -k4|  column -s, -t "$CSV_FILE" 
        ;;

    search)
        IFS=$'\n'
        risultati=$(grep $2 $CSV_FILE)
        if [ -z $risultati ]; then
            echo "Nessun risultato trovato."
        fi
        for entry in $risultati; do
            IFS=',' read -r -a campi <<< $entry
            echo "Nome: ${campi[0]}"
            echo "Cognome: ${campi[1]}"
            echo "Telefono: ${campi[2]}"
            echo "E-mail: ${campi[3]}"
            echo "Città: ${campi[4]}"
            echo "Indirizzo: ${campi[5]}"
        done
        ;;

    insert)
        echo -n "Nome: "
        read nome
        echo -n "Cognome: "
        read cognome
        echo -n "Telefono: "
        read telefono
        echo -n "E-mail: "
        read email
        echo -n "Città: "
        read citta
        echo -n "Indirizzo: "
        read indirizzo
        
        if grep -q "$email" "$CSV_FILE"; then
            echo "L'indirizzo e-mail $email è già presente nel database."
        fi
        echo "$nome,$cognome,$telefono,$email,$citta,$indirizzo" >> "$CSV_FILE"
    ;;

    delete)
        echo -n "Inserisci l'indirizzo e-mail dell'utente da eliminare: "
        read email
        if grep -q "$email" "$CSV_FILE"; then
            grep -v "$email" "$CSV_FILE" > "$CSV_FILE.tmp"
            mv "$CSV_FILE.tmp" "$CSV_FILE"
            echo "Deleted"
        else
            echo "Cannot find any record"
        fi
    
esac
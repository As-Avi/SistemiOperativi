#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
    float base, altezza;
    if(argc < 3){
        printf("Uso: %s <base> <altezza>\n", argv[0]);
        return EXIT_FAILURE;
    }
    base = atof(argv[1]);
    altezza = atof(argv[2]);
    if(base <= 0 || altezza <= 0){
        printf("Base e altezza devono essere numeri positivi.\n");
        return EXIT_FAILURE;
    }
    float area = (base * altezza) / 2;
    printf("L'area del triangolo con base %.2f e altezza %.2f è: %.2f\n", base, altezza, area);
    return 0;
}
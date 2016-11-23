#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO 1

#define VAZIO 'e'
#define INICIAL 'S'
#define FINAL '$'

int isTerminal(char t){
    return !isupper(t);
}

void readFile(char *out, char *path){
    char c;
    char *aux = malloc(sizeof(char));
    FILE *file;

    file = fopen(path, "r");

    if (file == NULL){
        printf("Nao foi possivel abrir o arquivo.\n");
        exit(0);
    } else {
        while ((c = fgetc(file)) != EOF){
            if (c != ' '){
                snprintf(aux, sizeof(aux), "%c", c);
                strncat(out, aux, sizeof(aux));
            }
        }
    }

    free(aux);
    fclose(file);
}

int main(int argc, char *argv[]){
    char *in = malloc(sizeof(char));
    readFile(in, argv[ARQUIVO]);
    printf("%s\n", in);

    return 0;
}
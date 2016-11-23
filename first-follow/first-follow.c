#include <stdio.h>
#include <stdlib.h>

#define ARQUIVO 1

#define VAZIO 'e'
#define INICIAL 'S'
#define FINAL '$'

int isTerminal(char t){
    return !isupper(t);
}

void treatFile(char *path){
    char c;
    FILE *file, *newFile;

    file = fopen(path, "r");
    newFile = fopen("novo.txt", "w");

    if (file == NULL){
        printf("Nao foi possivel abrir o arquivo.\n");
        exit(0);
    } else {
        while ((c=fgetc(file)) != EOF){
            if (c != ' ') fprintf(newFile, "%c", c);
        }
    }

    fclose(file);
    fclose(newFile);
}

void readFile(){
    
    FILE *file;

    file = fopen("novo.txt", "r");
    fclose(file);
}

int main(int argc, char *argv[]){
    treatFile(argv[ARQUIVO]);
    readFile();

    printf("%d\n", isTerminal('c'));
    return 0;
}
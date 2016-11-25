#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRSIZE 100

#define ARQUIVO 1

#define VAZIO 'e'
#define INICIAL 'S'
#define FINAL '$'

int numRules, numChars;
char *firstSet;

// Structs
struct SRules{
    char head;
    char *production;
};
typedef struct SRules Rules;

Rules *rules;

void newRules(){
    int i;
    rules = malloc(GRSIZE * sizeof(Rules));

    for (i = 0; i < GRSIZE; ++i){
        rules->head = 0;
        rules->production = NULL;
    }
}

void treatGrammar(char *in){

    char head;
    char *line = NULL;
    char *prod = NULL;

    while((line = strsep(&in, "\n")) != NULL){
        head = line[0];
        line = &line[2];

        while((prod = strsep(&line, "|")) != NULL){            
            rules[numRules].head = head;
            rules[numRules].production = malloc(100 * sizeof(char));
            memset(rules[numRules].production, '\0', 100);

            strcpy(rules[numRules].production, prod);
            numRules++;
        }
    }
}

void printGrammar(){
    int i;

    for (i = 0; i < numRules; i++){
        printf("%c -> %s\n", rules[i].head, rules[i].production);
    }
}

// File & Grammar Related
void readFile(char *path){
    char c, head;

    //char *buffer = (char *)malloc(1024*sizeof(char));
    char aux[2];
    char buffer[1024];

    memset(aux, '\0', sizeof(aux));
    memset(buffer, '\0', sizeof(buffer));

    FILE *file;

    file = fopen(path, "r");

    if (file == NULL){
        printf("Nao foi possivel abrir o arquivo.\n");
        exit(0);
    } else {
        while ((c = fgetc(file)) != EOF){
            if (c != ' '){
                snprintf(aux, sizeof(aux), "%c", c);
                strncat(buffer, aux, strlen(aux));
            }
            numChars++;
        }
    }

    fclose(file);
    
    treatGrammar(buffer);
}

int isTerminal(char t){
    return !isupper(t);
}

void append(char *firstSet, char c){}

char *getFirstSet(char c){}

void first(char c){
    int i;
    if (isTerminal(c)){
        append(firstSet, c);
    } else {
        for (i = 0; i < strlen(getFirstSet(c)); i++){
            printf("0k\n");
        }
    }
}

int main(int argc, char *argv[]){
    int i;    

    newRules();
    readFile(argv[ARQUIVO]);

    printGrammar();

    /*treatGrammar(in, rules);
    printGrammar(rules);*/

    /*free(rules);*/

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO 1

#define VAZIO 'e'
#define INICIAL 'S'
#define FINAL '$'

int numRules;
char *firstSet;

// Structs
struct SRules{
    char head;
    char *production;
};
typedef struct SRules Rules;

Rules *newRules(n){
    Rules *rules = malloc(n*sizeof(Rules));
    rules->head = ' ';
    rules->production = malloc(sizeof(Rules));
    return rules;
}

// File & Grammar Related
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

    fclose(file);
    free(aux);
}

void treatGrammar(char *in, Rules *rule){
    int i;

    char head;
    char *line = NULL;
    char *prod = NULL;

    while((line = strsep(&in, "\n")) != NULL){
        head = line[0];
        line = &line[2];

        while((prod = strsep(&line, "|")) != NULL){            
            rule[numRules].head = head;
            rule[numRules].production = prod;
            numRules++;
        }
    }

    free(line);
    free(prod);
}

void printGrammar(Rules *grammar){
    int i;

    for (i = 0; i < numRules; i++){
        printf("%c -> %s\n", grammar[i].head, grammar[i].production);
    }
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

    char *in = malloc(sizeof(char));
    readFile(in, argv[ARQUIVO]);

    Rules *rules = newRules(numRules);

    treatGrammar(in, rules);
    printGrammar(rules);
    
    free(in);
    /*free(rules);*/

    return 0;
}
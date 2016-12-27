#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO 1
#define GRSIZE 100
#define ERROR -1

#define INICIAL 'S'
#define FINAL '$'

int numRules, numChars, numT, numNT;

char *ravelSet;

// Struct & Struct Related Function
struct SRules{
    char head;
    char *rightSide;
};
typedef struct SRules Rules;

Rules *rules;

void newRules(){
    int i;
    rules = malloc(GRSIZE * sizeof(Rules));

    for (i = 0; i < GRSIZE; ++i){
        rules->head = 0;
        rules->rightSide = NULL;
    }
}

// File & Grammar Related
void treatGrammar(char *in){

    char head;
    char *line = NULL;
    char *prod = NULL;

    while((line = strsep(&in, "\n")) != NULL){
        head = line[0];
        line = &line[2];

        while((prod = strsep(&line, "|")) != NULL){            
            rules[numRules].head = head;
            rules[numRules].rightSide = malloc(100 * sizeof(char));
            memset(rules[numRules].rightSide, '\0', 100);

            strcpy(rules[numRules].rightSide, prod);
            numRules++;
        }
    }
}

void printGrammar(){
    int i;

    for (i = 0; i < numRules; i++){
        printf("%d: %c -> %s\n", i, rules[i].head, rules[i].rightSide);
    }
}

void readFile(char *path){
    char c, head;

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

// Auxiliar Functions
int isTerminal(char t){
    return !isupper(t);
}

int hasLambda(int prod){
    if (strcmp(rules[prod].rightSide, "e") == 0)
        return 1;
    return 0;
}

int hasLambdaInSet(char *set){
    int i;
    for (i = 0; i < strlen(set); i++)
        if (set[i] == 'e') return 1;
    return 0;
}

int hasFinalInSet(char *set){
    int i;
    for (i = 0; i < strlen(set); i++)
        if (set[i] == FINAL) return 1;
    return 0;
}

void append(char *set, char token){
    int i;

    char aux[2];
    memset(aux, '\0', sizeof(aux));

    for (i = 0; i < strlen(set); i++){
        if (set[i] == token) return;
    }
    snprintf(aux, sizeof(aux), "%c", token);
    strncat(set, aux, strlen(aux));
}

void unionSet(char *setA, char *setB){
    int i;
    for (i = 0; i < strlen(setB); i++){
        if (setB[i] != 'e') 
            append(setA, setB[i]);
    }
}

void unionSetLambda(char *setA, char *setB){
    int i;
    for (i = 0; i < strlen(setB); i++)
        append(setA, setB[i]);
}

void ravelTerminals(char *right){
    int i;
    for (i = 0; i < strlen(right); i++){
        if (isTerminal(right[i])){
            append(ravelSet, right[i]);
        }
    }
}

void ravelNotTerminals(char left){
    append(ravelSet, left);
}

void addFinalToRavel(){
    append(ravelSet, FINAL);
}

void createRavelSet(){
    int i;
    ravelSet = malloc(GRSIZE * sizeof(char));
    memset(ravelSet, '\0', GRSIZE);
    for (i = 0; i < numRules; i++){
        ravelTerminals(rules[i].rightSide);
        ravelNotTerminals(rules[i].head);
    }
}

void getNumTokens(){
    int i;
    for (i = 0; i < strlen(ravelSet); i++){
        if (isTerminal(ravelSet[i])) numT++;
        else numNT++;
    }
}

int getIndexT(char c){
    int i, index = 0;
    for (i = 0; i < strlen(ravelSet); i++){
        if (isTerminal(ravelSet[i])){
            if (ravelSet[i] == c){
                return index;
            } else {
                index++;
            }
        }
    }
}

int getIndexNT(char c){
    int i, index = 0;
    for (i = 0; i < strlen(ravelSet); i++){
        if (!isTerminal(ravelSet[i])){
            if (ravelSet[i] == c){
                return index;
            } else {
                index++;
            }
        }
    }
}

int main(int argc, char *argv[]){
    int i, j;    

    newRules();

    readFile(argv[ARQUIVO]);

    printf("GRAMMAR\n");

    printGrammar();

    createRavelSet();

    printf("\n");

    return 0;
}
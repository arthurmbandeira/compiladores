#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRSIZE 100

#define ARQUIVO 1

#define VAZIO 'e'
#define INICIAL 'S'
#define FINAL '$'

int numRules, numChars, numFirstSets;

char *ravelSet;

// Structs
struct SRules{
    char head;
    char *rightSide;
};
typedef struct SRules Rules;

Rules *rules;
Rules *firstSet;

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
        printf("%c -> %s\n", rules[i].head, rules[i].rightSide);
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

char *getFirstSet(char c){
    int i;
    for (i = 0; i < numFirstSets; i++){
        if (firstSet[i].head == c){
            return firstSet[i].rightSide;
        }
        
    }
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

void createFirst(char new){
    firstSet[numFirstSets].head = new;
    firstSet[numFirstSets].rightSide = malloc(GRSIZE * sizeof(char));
    memset(firstSet[numFirstSets].rightSide, '\0', GRSIZE);
    if (isTerminal(new)){
        append(firstSet[numFirstSets].rightSide, new);
    }
    numFirstSets++;
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

void createRavelSet(){
    int i;
    ravelSet = malloc(GRSIZE * sizeof(char));
    memset(ravelSet, '\0', GRSIZE);
    for (i = 0; i < numRules; i++){
        ravelTerminals(rules[i].rightSide);
        ravelNotTerminals(rules[i].head);
    }
}

void createFirstSets(){
    int i, j;
    int exists;
    firstSet = malloc(GRSIZE * sizeof(Rules));
    for (i = 0; i < strlen(ravelSet); i++){
        exists = 0;
        for (j = 0; j < numFirstSets; j++){
            if (firstSet[numFirstSets].head == ravelSet[i]){
                exists = 1;
                break;
            }
        }
        if (!exists) createFirst(ravelSet[i]);
    }
}

void addToFirst(int ruleIndex, char token){
    int i;
    for (i = 0; i < numFirstSets; i++){
        if (firstSet[i].head == rules[ruleIndex].head){
            append(firstSet[i].rightSide, token);
        }
        
    }
}

void first(int ruleIndex){
    int i;
    if (hasLambda(ruleIndex)){
        addToFirst(ruleIndex, 'e');
    } else {
        for (i = 0; i < strlen(rules[ruleIndex].rightSide); i++) {
            char *Y = getFirstSet(rules[ruleIndex].rightSide[i]);
            unionSet(getFirstSet(rules[ruleIndex].head), Y);
            if (!hasLambdaInSet(Y)) break;
        }
    }
}

int main(int argc, char *argv[]){
    int i, j;    

    newRules();

    readFile(argv[ARQUIVO]);

    createRavelSet();
    printf("ravel set: %s\n", ravelSet);
    createFirstSets();

    
    /*printGrammar();*/

    for (j = 0; j < 100; j++){
        for (i = 0; i < numRules; i++){
            first(i);
        }
    }
    /*addToFirst(0, 'k');*/

    for (i = 0; i < numFirstSets; i++){
        printf("first sets: %c -> %s\n", firstSet[i].head, firstSet[i].rightSide);
    }

    

    /*treatGrammar(in, rules);
    printGrammar(rules);*/

    /*free(rules);*/

    return 0;
}
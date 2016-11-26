#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO 1
#define GRSIZE 100

#define VAZIO 'e'
#define INICIAL 'S'
#define FINAL '$'

int numRules, numChars, numFirstSets, numFollowSets;

char *ravelSet;

// Structs
struct SRules{
    char head;
    char *rightSide;
};
typedef struct SRules Rules;

Rules *rules;
Rules *firstSet;
Rules *followSet;

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

char *getFollowSet(char c){
    int i;
    for (i = 0; i < numFollowSets; i++){
        if (followSet[i].head == c){
            return followSet[i].rightSide;
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

void unionSetLambda(char *setA, char *setB){
    int i;
    for (i = 0; i < strlen(setB); i++)
        append(setA, setB[i]);
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

void createFollow(char new){
    followSet[numFollowSets].head = new;
    followSet[numFollowSets].rightSide = malloc(GRSIZE * sizeof(char));
    memset(followSet[numFollowSets].rightSide, '\0', GRSIZE);
    numFollowSets++;
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
    int i;
    firstSet = malloc(GRSIZE * sizeof(Rules));
    for (i = 0; i < strlen(ravelSet); i++)
        createFirst(ravelSet[i]);
}

void createFollowSets(){
    int i;
    followSet = malloc(GRSIZE * sizeof(Rules));
    for (i = 0; i < strlen(ravelSet); i++){
        if (!isTerminal(ravelSet[i])){
            createFollow(ravelSet[i]);
        }
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

void addToFollow(int ruleIndex, char token){
    int i;
    for (i = 0; i < numFollowSets; i++){
        if (followSet[i].head == rules[ruleIndex].head){
            append(followSet[i].rightSide, token);
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

void follow(int ruleIndex){
    int i, j;
    if (rules[ruleIndex].head == INICIAL)
        addToFollow(ruleIndex, FINAL);
    else{
        for (i = 0; i < numRules; i++){
            for (j = 0; j < strlen(rules[i].rightSide); j++){
                if (!isTerminal(rules[i].rightSide[j])){
                    if (rules[i].rightSide[j+1] != '\0'){
                        char *firstBeta = getFirstSet(rules[i].rightSide[j+1]);
                        unionSet(getFollowSet(rules[i].rightSide[j]), firstBeta);
                        if (hasLambdaInSet(firstBeta)){
                            unionSetLambda(getFollowSet(rules[i].rightSide[j]), getFollowSet(rules[i].head));
                        }
                    } else {
                        unionSetLambda(getFollowSet(rules[i].rightSide[j]), getFollowSet(rules[i].head));
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]){
    int i, j;    

    newRules();

    readFile(argv[ARQUIVO]);

    createRavelSet();
    printf("ravel set: %s\n", ravelSet);
    printf("\n");

    createFirstSets();

    for (i = 0; i < 1000; i++){
        for (j = 0; j < numRules; j++){
            first(j);
        }
    }
    printf("\n");
    /*addToFirst(0, 'k');*/

    for (i = 0; i < numFirstSets; i++){
        printf("first sets: %c -> %s\n", firstSet[i].head, firstSet[i].rightSide);
    }

    printf("\n");

    createFollowSets();

    for (i = 0; i < 1000; i++){
        for (j = 0; j < numRules; j++){
            follow(j);
        }
    }

    for (i = 0; i < numFollowSets; i++){
        printf("follow sets: %c -> %s\n", followSet[i].head, followSet[i].rightSide);
    }

    

    /*treatGrammar(in, rules);
    printGrammar(rules);*/

    /*free(rules);*/

    return 0;
}
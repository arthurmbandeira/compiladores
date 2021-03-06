#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO 1
#define GRSIZE 100
#define ERROR -1

#define INICIAL 'S'
#define FINAL '$'

#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

int numRules, numChars, numT, numNT;

char *ravelSet, *ravelNonTSet;

// Structs & Struct Related Functions

typedef struct Elem{
    char head;
    char *prod;
    struct Elem *next;
} Elem;

typedef struct Queue{
    Elem *begin;
    Elem *end;
    int size;
} Queue;

Elem *newElem(char head, char *prod){
    Elem *elem = malloc(sizeof(Elem));
    elem->head = head;
    elem->prod = prod;
    elem->next = NULL;
    return elem;
}

Queue *newQueue(){
    Queue *q = malloc(sizeof(Queue));
    q->begin = NULL;
    q->end = NULL;
    q->size = 0;
    return q;
}

// Queue & Element related functions

int emptyQueue(Queue *Q){
    if (Q->begin == NULL)
        return 1;
    return 0;
}

Elem *getElem(char head, Queue *Q){
    if (emptyQueue(Q)) return NULL;

    Elem *aux = Q->begin;
    while (aux != NULL && aux->head != head)
        aux = aux->next;
    return aux;
}

void insertElem(char head, char *prod, Queue *Q){
    Elem *e = newElem(head, prod);

    if (emptyQueue(Q)){
        Q->begin = e;
        Q->end = e;
    } else {
        Q->end->next = e;
        Q->end = e;
    }
    Q->size++;
}

int insertElemInPosition(char head, char *prod, Queue *Q, char pos){
    Elem *e = newElem(head, prod);
    Elem *p = getElem(pos, Q);

    if (p == NULL) return 0;
    if (p->next == NULL){
        p->next = e;
        e->next = NULL;
        Q->end = e;
    } else {
        e->next = p->next;
        p->next = e;
    }
    Q->size++;
    return 1;
}

int existsHeadInQueue(char x, Queue *Q){
    Elem *e = Q->begin;
    while (e != NULL){
        if (e->head == x) return 1;
        e = e->next;
    }
    return 0;
}

void printElem(Elem *elem){
    Elem *aux;
    for (aux = elem; aux != NULL; aux = aux->next)
        printf("%c - %s\n", aux->head, aux->prod);
}

void orderQueue(Queue *order, Queue *grammar){
    Elem *e = grammar->begin;
    while(e != NULL){
        if (!existsHeadInQueue(e->head, order))
            insertElem(e->head, NULL, order);
        e = e->next;
    }
}

// File & Grammar Related
void treatGrammar(char *in, Queue *grammar){
    char head;
    char *line = NULL;
    char *prod = NULL;

    char *newProd = NULL;

    while((line = strsep(&in, "\n")) != NULL){
        head = line[0];
        line = &line[2];

        while((prod = strsep(&line, "|")) != NULL){
            newProd = malloc(GRSIZE * sizeof(char));
            memset(newProd, '\0', 100);
            strcpy(newProd, prod);

            insertElem(head, prod, grammar);
        }
    }
}

void printGrammar(Queue *grammar){
    Elem *e = grammar->begin;
    while (e != NULL){
        printf("%c -> %s\n", e->head, e->prod);
        e = e->next;
    }
}

void printOrder(Queue *order){
    Elem *e = order->begin;
    int i = 0;
    while (e != NULL){
        printf("%d : %c\n", i, e->head);
        e = e->next;
        i++;
    }
}


void readFile(char *path, Queue *Q){
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
    
    treatGrammar(buffer, Q);
}

// Auxiliar Functions
int isTerminal(char t){
    return !isupper(t);
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

int main(int argc, char *argv[]){
    int i, j;

    Queue *grammar = newQueue();
    Queue *order = newQueue();

    readFile(argv[ARQUIVO], grammar);

    printf("GRAMMAR\n");
    printGrammar(grammar);
    printf("\n");

    printf("ORDER\n");
    orderQueue(order, grammar);
    printOrder(order);
    printf("\n");

    return 0;
}
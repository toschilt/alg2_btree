#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdio.h>
#include <stdlib.h>

//TAMANHO DA PÁGINA: 4096 Bytes
#define PAGESIZE 4096
//Definir tamanho de páginas bonitinho

typedef struct {
    //Estrutura para armazenar possíveis erros (Ideia tirada do PDF do trabalho)
} Errors;


typedef struct {
    int key, RRN;
} record;

typedef struct {
    record *records;
    long *childs;
    int keys;
    char isLeaf;
} bTreePage;

typedef struct {
    int key;
    long RRN;
    long childs[2];
} promotedKey;


//Talvez criar outro arquivo para cuidar de funções específicas, como overflow e carregar raiz da árvore
bTreePage *getRootBTree(FILE *bTreeFile);
Errors bTreeInsert(record *record, bTreePage *root, FILE *bTreeFile);
Errors bTreeUpdate(record *record, bTreePage *root, FILE *bTreeFile);
long bTreeSearch(bTreePage *root, int key, FILE *bTreeFile);



#endif
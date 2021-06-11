#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdio.h>
#include <stdlib.h>

//TAMANHO DA PÁGINA: 4096 Bytes
#define PAGESIZE 4096
#define MAXKEYS 5
// #define QUANTIDADE DE REGISTRO

typedef struct {
    int key;
    long RRN;
} record;

typedef struct {
    record records[MAXKEYS-1];
    long childs[MAXKEYS];
    int numRecords;
    char isLeaf;
} bTreePage;

typedef struct {
    int key;
    long RRN;
    long childs[2];
} promotedKey;


bTreePage *createRoot(FILE *bFile);
bTreePage *getOrCreateRoot(FILE *bFile);

long bTreeSearch(int key);
int _bTreeSearch(bTreePage *bPage, int searchKey);
long pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch);
long binarySearchForInsertion(int searchKey, record *records, long firstSearch, long lastSearch);

int bTreeInsert(record *newRecord);
int _bTreeInsert(record *newRecord, bTreePage *bPage, promotedKey **promoted);

promotedKey *bTreeInsertIntoPage(record *newRecord, promotedKey *promoted, bTreePage *bPage, long insertPosition);
int headerUpdate(promotedKey *promoted);

void printNode(bTreePage *bPage);
//Funções para update


#endif
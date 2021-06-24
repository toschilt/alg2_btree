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
    record *rec;
    long childs[2];
} promotedKey;

typedef struct {
    bTreePage *bPage;
    int RRN;
} newPageInfo;


newPageInfo *createRoot(FILE *bFile);
newPageInfo *getOrCreateRoot(FILE *bFile);

long bTreeSearch(int key);
long _bTreeSearch(newPageInfo *newPage, int searchKey);
long pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch);

int bTreeInsert(record *newRecord);
int _bTreeInsert(record *newRecord, newPageInfo *newPage, promotedKey **promoted);

void bTreeInsertIntoPage(record *newRecord, promotedKey **promoted, newPageInfo *newPage, long insertPosition);
int headerUpdate(promotedKey *promoted, FILE *bFile);

void printNode(bTreePage *bPage);


#endif
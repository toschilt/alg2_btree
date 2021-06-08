#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdio.h>
#include <stdlib.h>

//TAMANHO DA PÁGINA: 4096 Bytes
#define PAGESIZE 4096
#define MAXKEYS 204
// #define QUANTIDADE DE REGISTRO

typedef struct {
    int success;
    //Estrutura para armazenar possíveis erros (Ideia tirada do PDF do trabalho)
} Errors;

typedef struct {
    int key;
    long RRN;
} record;

typedef struct {
    record *records;
    long *childs;
    int numRecords;
    char isLeaf;
} bTreePage;

typedef struct {
    int key;
    long RRN;
    long childs[2];
} promotedKey;

typedef struct {
    //Sei la pra que que serve esse role
} PrimaryIndex;


bTreePage *createRoot(FILE *bFile);
bTreePage *getOrCreateRoot(FILE *bFile);

long bTreeSearch(int key);
bTreePage *_bTreeSearch(bTreePage *bPage, int searchKey);
record *pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch);

int bTreeInsert(record *newRecord);
int *_bTreeInsert(int key, bTreePage *bPage, promotedKey **promoted);

promotedKey *bTreeInsertIntoPage(promotedKey *promoted, bTreePage *bPage, long insertPosition);
long binarySearchForInsertion(int searchKey, record *records, long firstSearch, long lastSearch);
int headerUpdate(promotedKey *promoted);

//Funções para update


//Funções antigas:
/* Coleta o nó raiz
 * Chama a função de busca
 * Caso seja encontrado, re-escreve as informações no nó e salva o arquivo
 * Caso não seja encontrado retorna erro
*/ 
//Errors bTreeUpdate(record *record, bTreePage *root, FILE *bTreeFile);

/* Coleta o nó raíz
 * Busca pela chave na página
 * Caso não encontre, busca na subárvore apropriada
 * Recursão até encontrar a chave 
*/
//long bTreeSearch(bTreePage *root, int key, FILE *bTreeFile);







#endif
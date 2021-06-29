#ifndef __BTREEUTILS_H__
#define __BTREEUTILS_H__

/*
 * Algoritmos e Estruturas de Dados II
 * Autores: Lucas Toschi, Murilo Todão e Marcos Ordonha (Grupo 15)
 * 
 * BTREEUTILS.H
 * Neste arquivo estão todas as funções e estruturas que estão
 * relacionadas com o funcionamento geral intrínseco da BTree.
*/


#include <stdio.h>
#include <stdlib.h>

#define PAGESIZE 4096
#define MAXCHILDS 171


/*
 * Struct responsável por armazenar cada registro.
 * Composta por uma chave (número inteiro) e o RRN correspondente.
*/
typedef struct {
    int key;
    long RRN;
} record;

/*
 * Struct responsável por a página da BTree.
 * É composta pela struct "record", por um vetor de RRN filhos,
 * o número de registros na página e uma flag indicando se é
 * uma página folha ou não.
*/
typedef struct {
    record records[MAXCHILDS-1];
    long childs[MAXCHILDS];
    short numRecords;
    char isLeaf;
} bTreePage;

/*
 * Struct responsável por armazenar um registro que será promovido.
 * É composta por um ponteiro de registro e os RRN filhos do registro
 * em questão.
*/
typedef struct {
    record *rec;
    long childs[2];
} promotedKey;

/*
 * Struct responsável pelo armazenamento da página da BTree.
 * É composta pela página da BTree que será acessada e o RRN
 * correspondente no arquivo de dados.
*/
typedef struct {
    bTreePage *bPage;
    long RRN;
} bPageInfo;



/*
 * Função responsável pela criação de uma página da BTree.
*/
bTreePage *createPage();

/*
 * Função responsável por criar a raíz da BTree.
*/
bPageInfo *createRoot(FILE *bFile);

/*
 * Função responsável por encontrar ou criar a raíz da BTree.
*/
bPageInfo *getOrCreateRoot(FILE *bFile);

/*
 * Função que realiza a busca binária na página da BTree a partir do vetor
 * de registros e da chave em questão (número inteiro).
*/
long pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch);


/*
 * Funções responsáveis por imprimir informações da árvore
*/
void printNode(bPageInfo *bPage);
void printPromoted(promotedKey *promoted);
void printArray(long *childsArray);


#endif
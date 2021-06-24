#ifndef __BTREE_H__
#define __BTREE_H__

/*
 * Algoritmos e Estruturas de Dados II
 * Autores: Lucas Toschi e Murilo Todão (Grupo 7)
 * 
 * BTREE.H
 * Neste arquivo estão todas as funções e estruturas que estão
 * relacionadas com o funcionamento intrínseco da BTree.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "streamHandler.h"


#define PAGESIZE 4096
#define MAXKEYS 5

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
    record records[MAXKEYS-1];
    long childs[MAXKEYS];
    int numRecords;
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
    int RRN;
} newPageInfo;



//INICIALIZAÇÃO E MANUTENÇÃO DA BTREE
/*
 * Função responsável pela criação de uma página da BTree.
*/
bTreePage *createPage();

/*
 * Função responsável por criar a raíz da BTree.
*/
newPageInfo *createRoot(FILE *bFile);

/*
 * Função responsável por encontrar ou criar a raíz da BTree.
*/
newPageInfo *getOrCreateRoot(FILE *bFile);



//BUSCA NA BTREE
/*
 * Função que inicia a busca na BTree a partir de uma
 * chave (número inteiro).
 * Retorna o RRN no registro buscado.
*/
long bTreeSearch(int key);

/*
 * Função recursiva que percorre a árvore a partir de uma
 * chave (número inteiro) e da página a ser buscada.
 * Retorna o índice do registro buscado.
*/
int _bTreeSearch(newPageInfo *newPage, int searchKey);

/*
 * Função que realiza a busca binária na página da BTree a partir do vetor
 * de registros e da chave em questão (número inteiro).
*/
long pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch);

/*
 * Função que realiza a busca binária para inserção na página da BTree
 * a partir do vetor de registros e da chave em questão (número inteiro).
*/
long binarySearchForInsertion(int searchKey, record *records, long firstSearch, long lastSearch);



//INSERÇÃO NA BTREE
/*
 * Função que inicia a inserção na BTree a partir de um registro
 * específico
*/
int bTreeInsert(record *newRecord);

/*
 * Função recursiva que percorre a árvore para inserção de um registro.
 * Já realiza o tratamento de nós promovidos.
*/
int _bTreeInsert(record *newRecord, newPageInfo *newPage, promotedKey **promoted);

/*
 * Função que realiza a inserção de um registro em uma página específica da BTree.
*/
void bTreeInsertIntoPage(record *newRecord, promotedKey **promoted, newPageInfo *newPage, long insertPosition);

/*
 * Função responsável pela atualização da página header da BTree.
*/
int headerUpdate(promotedKey *promoted, FILE *bFile);

/*
 * Função responsável por imprimir uma página da BTree para o usuário.
*/
void printNode(bTreePage *bPage);


#endif
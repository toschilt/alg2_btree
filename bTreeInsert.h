#ifndef __BTREEINSERT_H__
#define __BTREEINSERT_H__

#include "bTreeUtils.h"

/*
 * Algoritmos e Estruturas de Dados II
 * Autores: Lucas Toschi, Murilo Todão e Marcos Ordonha (Grupo 15)
 * 
 * BTREEINSERT.H
 * Neste arquivo estão todas as funções e estruturas que estão
 * relacionadas com a inserção de registros na BTree.
*/


/*
 * Função que inicia a inserção na BTree a partir de um registro
 * específico
*/
int bTreeInsert(record *newRecord);

/*
 * Função recursiva que percorre a árvore para inserção de um registro.
 * Já realiza o tratamento de nós promovidos.
*/
int _bTreeInsert(record *newRecord, bPageInfo *bInfo, promotedKey **promoted);

/*
 * Função que realiza a inserção de um registro em uma página específica da BTree.
*/
void bTreeInsertIntoPage(record *newRecord, promotedKey **promoted, bPageInfo *bInfo, long insertPosition);

/*
 * Funções para limpar informações da página com overflow e inserir informações na página nova, respectivamente
*/
void insertPageData(bTreePage *bPage, bTreePage *createdPage, long *childsArray, long startingPosition);
void cleanPageData(bTreePage *bPage, long *childsArray, long startingPosition);

/*
 * Função para promover um novo record em caso de overflow
*/
promotedKey *promoteKey(record *rec, int LeftRRN, int RightRRN);

/*
 * Função para armazenar informações acerca de filhos em caso de overflow
*/
long *createChildsArray(bPageInfo *bInfo, promotedKey *promoted);

/*
 * Função responsável pela atualização da página header da BTree.
*/
int headerUpdate(promotedKey *promoted, FILE *bFile);



#endif
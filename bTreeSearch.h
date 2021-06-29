#ifndef __BTREESEARCH_H__
#define __BTREESEARCH_H__

#include "bTreeUtils.h"

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
long _bTreeSearch(bPageInfo *newPage, int searchKey);



#endif
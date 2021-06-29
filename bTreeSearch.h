#ifndef __BTREESEARCH_H__
#define __BTREESEARCH_H__

#include "bTreeUtils.h"

/*
 * Algoritmos e Estruturas de Dados II
 * Autores: Lucas Toschi, Murilo Todão e Marcos Ordonha (Grupo 15)
 * 
 * BTREESEARCH.H
 * Neste arquivo estão todas as funções e estruturas que estão
 * relacionadas com a parte de busca da BTree.
*/


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
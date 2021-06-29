#ifndef __PUBLICINTERFACE_H__
#define __PUBLICINTERFACE_H__

/*
 * Algoritmos e Estruturas de Dados II
 * Autores: Lucas Toschi, Murilo Todão e Marcos Ordonha (Grupo 15)
 * 
 * PUBLICINTERFACE.H
 * Neste arquivo estão todas as funções que são chamadas
 * quando o usuário precisa realizar alguma das funções
 * disponíveis pelo programa: busca, inserção, atualização
 * e impressão.
*/


#include "student.h"
#include "bTreeUtils.h"
#include "bTreeSearch.h"
#include "bTreeInsert.h"
#include "streamHandler.h"


/*
 * Função de busca a partir de uma chave númerica (inteiro).
*/
void search(int key);

/*
 * Função de inserção a partir das informações de um estudante
 * organizadas na struct "studentRegister".
*/
void insert(studentRegister *student);

/*
 * Função de atualização a partir das informações de um estudante
 * organizadas na struct "studentRegister".
*/
void update(studentRegister *student);

/*
 * Função responsável por imprimir a árvore por completo para o usuário
*/
void print();


#endif
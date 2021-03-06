#ifndef __STREAMHANDLER_H__
#define __STREAMHANDLER_H__

/*
 * Algoritmos e Estruturas de Dados II
 * Autores: Lucas Toschi, Murilo Todão e Marcos Ordonha (Grupo 15)
 * 
 * STREAMHANDLER.H
 * Neste arquivo estão todas as funções que tem alguma relação
 * com manipulação de streams, seja stdin ou arquivos.
*/


#include "student.h"
#include "bTreeUtils.h"
#include <string.h>

#define BTREEFILENAME "btree.dat"
#define DATAFILENAME "data.dat"


/*
 * Leitura de string do usuário a partir da entrada padrão.
*/
void readString(char *string, char separator);

/*
 * Busca do registro no arquivo de dados a partir do RNN
 * Retorna as infromações no formato da struct "studentRegister".
*/
studentRegister *searchStudentInDataFile(long RRN);

/*
 * Função responsável por ler as informações do estudante.
 * Retorna as informações no formato da struct "studentRegister".
*/
studentRegister *readStudentFromUser();

/*
 * Função responsável pela inserção da struct "studentRegister"
 * no arquivo de dados.
*/
void insertStudentInDataFile(studentRegister *student);

/*
 * Função responsável pela atualização das informações do estudante
 * (compiladas na struct "studentRegister") no arquivo de dados a
 * partir do RNN
*/
void updateStudent(studentRegister *student, long RRN);

/*
 * Insere um novo nó da árvore BTree no arquivo correspondente.
*/
void insertNodeInBTreeFile(bPageInfo *newPage, FILE *bFile, long RRN);

/*
 * Busca um registro no arquivo de dados. Retorna o RNN correspondente.
*/
long getRecordsInDataFile();

/*
 * Recupera a página do arquivo da BTree.
*/
bPageInfo *getPageFromBTreeFile(long RRN);


#endif
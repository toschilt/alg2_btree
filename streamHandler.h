#ifndef __STREAMHANDLER_H__
#define __STREAMHANDLER_H__

#include "student.h"
#include "btree.h"


studentRegister *searchInDataFile(long RRN);
studentRegister *readStudentFromUser();

void insertStudentInDataFile(studentRegister *student);
void insertNodeInBTreeFile(); //DISCUTIR SOBRE ESSA FUNÇÃO

bTreePage *getPageFromBTreeFile(long RRN);

//Função para salvar registro em arquivo de dados

//Função para coletar registro do arquivo de dados a partir do RRN

// insertInDataFile(student)
// searchInDataFile(RRN)
// getPageFromBTreeFile(RRN)



#endif
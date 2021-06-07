#ifndef __STREAMHANDLER_H__
#define __STREAMHANDLER_H__

#include "student.h"
#include "btree.h"


studentRegister *searchInDataFile(long RRN);
studentRegister *readStudentFromUser();


//Função para salvar registro em arquivo de dados

//Função para coletar registro do arquivo de dados a partir do RRN

//Cuidar de strings de entrada (Talvez em arquivo separado?)

#endif
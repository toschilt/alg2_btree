#ifndef __STREAMHANDLER_H__
#define __STREAMHANDLER_H__

#include "student.h"
#include "btree.h"

#define EXITFUNCTIONNAME "exit"
#define SEARCHFUNCTIONNAME "search"
#define INSERTFUNCTIONNAME "insert"
#define UPDATEFUNCTIONNAME "update"


void getFunction();

studentRegister *searchInDataFile(long RRN);
studentRegister *readStudentFromUser();


//Função para salvar registro em arquivo de dados

//Função para coletar registro do arquivo de dados a partir do RRN

//Cuidar de strings de entrada (Talvez em arquivo separado?)

#endif
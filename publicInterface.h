#ifndef __PUBLICINTERFACE_H__
#define __PUBLICINTERFACE_H__

#include "student.h"
#include "btree.h"
#include "streamHandler.h"

void search(long key);
void insert(studentRegister *student);
void update(studentRegister *student);


/*FUNÇÕES
    -- SEARCH --
    Chama bTreeSearch(chave)
    recebe RRN
    se RRN for -1, retorna que registro nao existe
    searchInDataFile(rrn)
    pritntStudent(student)


    -- INSERT --
    inserir na btree
    imprime caso o registro ja exista
*/

#endif
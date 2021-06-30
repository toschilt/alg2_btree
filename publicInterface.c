#include "publicInterface.h"


void search(int key) {
    long RRN = bTreeSearch(key);

    if(RRN == -1) {
        printf("Registro nao encontrado!\n");
        return;
    }

    studentRegister *student = searchStudentInDataFile(RRN);
    printStudent(student);
}


void insert(studentRegister *student) {
    
    record *rec = (record*)malloc(sizeof(record));
    rec->key = student->nusp;
    rec->RRN = getRecordsInDataFile();
    int error = bTreeInsert(rec);

    if(error == -1) {
        printf("O Registro ja existe!\n");
        return;
    }

    insertStudentInDataFile(student);
}


void update(studentRegister *student) {
    long RRN = bTreeSearch(student->nusp);

    if(RRN == -1) {
        printf("Registro nao encontrado!\n");
        return;
    }

    updateStudent(student, RRN);
}
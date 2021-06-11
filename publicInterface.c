#include "publicInterface.h"


void search(int key) {
    long RRN = bTreeSearch(key);

    if(RRN == -1) {
        printf("O registro não existe\n");
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

    if(error) {
        printf("O registro já existe\n");
        return;
    }

    insertStudentInDataFile(student);
}


void update(studentRegister *student) {
    printf("Atualização\n");
}


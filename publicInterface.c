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

    if(error) {
        printf("O registro já existe\n");
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



void printTree(newPageInfo *newPage) {
    printf("\n");
    printNode(newPage->bPage);

    if(!newPage->bPage->isLeaf) {
        printf("\n\n");
        for(int i = 0; i <= newPage->bPage->numRecords; i++) {
            newPageInfo *nP = getPageFromBTreeFile(newPage->bPage->childs[i]);
            printTree(nP);
        }
    }
}


void print() {
    FILE *fp = fopen(BTREEFILENAME, "r+");
    newPageInfo *newPage = getOrCreateRoot(fp);
    printTree(newPage);
}
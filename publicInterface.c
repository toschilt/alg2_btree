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



void printTree(bPageInfo *bInfo) {
    printf("\n");
    printNode(bInfo);

    if(!bInfo->bPage->isLeaf) {
        printf("\n");
        for(int i = 0; i <= bInfo->bPage->numRecords; i++) {
            printf("\n");
            bPageInfo *nP = getPageFromBTreeFile(bInfo->bPage->childs[i]);
            printTree(nP);
        }
    }
}


void print() {
    FILE *fp = fopen(BTREEFILENAME, "r+");
    bPageInfo *bInfo = getOrCreateRoot(fp);
    printTree(bInfo);
}
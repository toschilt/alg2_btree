#include "btree.h"
#include "streamHandler.h"
#include "math.h"


bTreePage *createPage() {
    bTreePage *bPage = (bTreePage*)malloc(PAGESIZE);
    bPage->numRecords = 0;
    bPage->isLeaf = 1;
    
    for(int i = 0; i < MAXKEYS; i++) {
        bPage->childs[i] = -1;
        if(i != (MAXKEYS-1)) { 
            bPage->records[i].key = 0;
            bPage->records[i].RRN = -1;
        }
    }
    return bPage;
}


bTreePage *createRoot(FILE *bFile) {
    bTreePage *bPage = createPage();
    int headerRRN = 1, created = -1;
    
    fseek(bFile, 0, SEEK_SET);
    fwrite(&created, sizeof(int), 1, bFile);
    fwrite(&headerRRN, PAGESIZE - sizeof(int), 1, bFile);
    fflush(bFile);
    
    insertNodeInBTreeFile(bPage, bFile, headerRRN);
    return bPage;
}


bTreePage *getOrCreateRoot(FILE *bFile) {
    int headerRRN, created;
    fseek(bFile, 0, SEEK_SET);
    fread(&created, sizeof(int), 1, bFile);

    if(created != -1) { 
        bTreePage *bPage = createRoot(bFile); 
        return bPage;
    } else {
        fread(&headerRRN, sizeof(int), 1, bFile);
        bTreePage *bPage = getPageFromBTreeFile(headerRRN);
        return bPage;
    }
}


long pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch) {
    long middle = (firstSearch + lastSearch) / 2;

    // printf("first: %ld last: %ld middle %ld\nkey: %d rkey: %d\n\n", firstSearch, lastSearch, middle, searchKey, records[middle].key);
  
    if(records[middle].key == searchKey) { return middle; }
    if(middle == firstSearch && middle == lastSearch) { return -1; }

    if(records[middle].key < searchKey) { 
        return pageBinarySearch(searchKey, records, middle+1, lastSearch);
    }

    return pageBinarySearch(searchKey, records, firstSearch, middle); 
}

//TALVEZ JUNTAR AMBAS AS FUNÇÕES DE BUSCA BINÁRIA EM 1 SÓ, COM O FLAG DE TIPO DE OPERAÇÃO, DADO QUE SÃO EXATAMENTE O MSM CÓDIGO
long binarySearchForInsertion(int searchKey, record *records, long firstSearch, long lastSearch) {
    long middle = (firstSearch + lastSearch) / 2;

    // printf("first: %ld last: %ld middle %ld\nkey: %d rkey: %d\n\n", firstSearch, lastSearch, middle, searchKey, records[middle].key);
    
    if(records[middle].key == searchKey) { return -1; }
    if(middle == firstSearch && middle == lastSearch) { return middle; }

    if(records[middle].key < searchKey) { 
        return binarySearchForInsertion(searchKey, records, middle+1, lastSearch);
    }

    return binarySearchForInsertion(searchKey, records, firstSearch, middle);  
}


long bTreeSearch(int searchKey) {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    bTreePage *bPage = getOrCreateRoot(bFile);

    int elementPosition = _bTreeSearch(bPage, searchKey);
    if(elementPosition == -1) { return -1; } //Não encontrou a chave
    return bPage->records[elementPosition].RRN; //Retorna o RRN da chave encontrada
}


int _bTreeSearch(bTreePage *bPage, int searchKey) {
    long elementPosition = pageBinarySearch(searchKey, bPage->records, 0, bPage->numRecords);

    if(bPage->records[elementPosition].key == searchKey) { return elementPosition; }

    if(!bPage->isLeaf) { //Caso não seja folha
        if(searchKey < bPage->records[elementPosition].key) {
            //Se chave de busca < registro em insertPoint, chama recursão no filho a esquerda
            bTreePage *searchPage = getPageFromBTreeFile(bPage->records[elementPosition].RRN);
            return _bTreeSearch(searchPage, searchKey);
        } else {
            //Se chave de busca > registro em insertPoint, chama recursão no filho a direita
            bTreePage *searchPage = getPageFromBTreeFile(bPage->records[elementPosition+1].RRN);
            return _bTreeSearch(searchPage, searchKey);
        }
    }

    else { return -1; } //Chave não encontrada
}


void printNode(bTreePage *bPage) {
    for(int i = 0; i < MAXKEYS; i++) {
        printf("CHILD: %ld ", bPage->childs[i]);
        
        if(i != (MAXKEYS-1)) { 
            printf("KEY: %d ", bPage->records[i].key);
            printf("RRN: %ld\n", bPage->records[i].RRN);
        } else { printf("\n"); }
    }
}


int bTreeInsert(record *newRecord) {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    bTreePage *bPage = getOrCreateRoot(bFile);
    
    promotedKey *promoted = (promotedKey*)malloc(sizeof(promotedKey));
    promoted = NULL;

    int error = _bTreeInsert(newRecord, bPage, &promoted);

    if(error == 1) { return 1; }
    if(promoted != NULL) { headerUpdate(promoted); }

    insertNodeInBTreeFile(bPage, bFile, 1); //Vai ser necessário mudar no futuro

    fclose(bFile);
    free(bPage);
    return 0;
}


int _bTreeInsert(record *newRecord, bTreePage *bPage, promotedKey **promoted) {

    int insertPoint = binarySearchForInsertion(newRecord->key, bPage->records, 0, bPage->numRecords);
    if(insertPoint == -1) { return 1; } //Chave foi encontrada

    if(!bPage->isLeaf) { //Caso não seja folha
        if(newRecord->key < bPage->records[insertPoint].key) {
            //Se chave de busca < registro em insertPoint, chama recursão no filho a esquerda
            bTreePage *searchPage = getPageFromBTreeFile(bPage->records[insertPoint].RRN);
            return _bTreeInsert(newRecord, searchPage, promoted);
        } else {
            //Se chave de busca > registro em insertPoint, chama recursão no filho a direita
            bTreePage *searchPage = getPageFromBTreeFile(bPage->records[insertPoint+1].RRN);
            return _bTreeInsert(newRecord, searchPage, promoted);
        }
    }

    else {
        *promoted = bTreeInsertIntoPage(newRecord, *promoted, bPage, insertPoint);
    }

    return 0;
}


promotedKey *bTreeInsertIntoPage(record *newRecord, promotedKey *promoted, bTreePage *bPage, long insertPosition) {

    if(bPage->numRecords == MAXKEYS) {
        //OVERFLOW 
    } 

    else {
        //Falta atualizar os filhos!!!
        for(int i = (MAXKEYS-2); i > insertPosition; i--) { 
            bPage->records[i].key = bPage->records[i-1].key;
            bPage->records[i].RRN = bPage->records[i-1].RRN;
        }
        bPage->records[insertPosition].key = newRecord->key;
        bPage->records[insertPosition].RRN = newRecord->RRN;
    }

    bPage->numRecords++;
    return promoted;
}


int headerUpdate(promotedKey *promoted) {
    return 0;
}


void bTreePrint() {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    bTreePage *bPage = getOrCreateRoot(bFile);
    printNode(bPage);
    free(bPage);
}
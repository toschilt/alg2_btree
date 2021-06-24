#include "btree.h"
#include "streamHandler.h"
#include "math.h"


// *Função utilizada para criar uma nova página da bTree
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


// *Função utilizada para criar a raíz do arquivo da bTree
newPageInfo *createRoot(FILE *bFile) {
    newPageInfo *newPage = (newPageInfo*)malloc(sizeof(newPageInfo));
    newPage->bPage = createPage();
    newPage->RRN = 1;
    int created = -1;
    
    fseek(bFile, 0, SEEK_SET);
    fwrite(&created, sizeof(int), 1, bFile);
    fwrite(&newPage->RRN, PAGESIZE - sizeof(int), 1, bFile);
    fflush(bFile);
    
    insertNodeInBTreeFile(newPage, bFile, newPage->RRN);
    return newPage;
}


// *Função utilizada para coletar a raíz
newPageInfo *getOrCreateRoot(FILE *bFile) {
    int created, RRN;
    fseek(bFile, 0, SEEK_SET);
    fread(&created, sizeof(int), 1, bFile);

    if(created != -1) {
        newPageInfo *newPage = createRoot(bFile); 
        return newPage;
    } else {
        fread(&RRN, sizeof(int), 1, bFile);
        newPageInfo *newPage = getPageFromBTreeFile(RRN);
        return newPage;
    }
}


long pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch) {
    long middle = (firstSearch + lastSearch) / 2;

    if(records[middle].key == searchKey || (middle == firstSearch && middle == lastSearch)) { return middle; }

    if(records[middle].key < searchKey) { 
        return pageBinarySearch(searchKey, records, middle+1, lastSearch);
    }

    return pageBinarySearch(searchKey, records, firstSearch, middle); 
}


long bTreeSearch(int searchKey) {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    newPageInfo *newPage = getOrCreateRoot(bFile);

    return _bTreeSearch(newPage, searchKey);
}


long _bTreeSearch(newPageInfo *newPage, int searchKey) {
    long elementPosition = pageBinarySearch(searchKey, newPage->bPage->records, 0, newPage->bPage->numRecords);

    if(newPage->bPage->records[elementPosition].key == searchKey) { return newPage->bPage->records[elementPosition].RRN; }

    if(!newPage->bPage->isLeaf) { //Caso não seja folha
        newPageInfo *searchPage = getPageFromBTreeFile(newPage->bPage->childs[elementPosition]);
        printNode(searchPage->bPage);
        return _bTreeSearch(searchPage, searchKey);
    }

    else { return -1; } //Chave não encontrada
}


int bTreeInsert(record *newRecord) {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    newPageInfo *newPage = getOrCreateRoot(bFile);
     
    promotedKey *promoted = NULL;

    int error = _bTreeInsert(newRecord, newPage, &promoted);

    if(error == 1) { return 1; }
    if(promoted != NULL) { headerUpdate(promoted, bFile); }

    insertNodeInBTreeFile(newPage, bFile, newPage->RRN);

    fclose(bFile);
    free(newPage);
    return 0;
}


int _bTreeInsert(record *newRecord, newPageInfo *newPage, promotedKey **promoted) {

    int insertPoint = pageBinarySearch(newRecord->key, newPage->bPage->records, 0, newPage->bPage->numRecords);
    if(newPage->bPage->records[insertPoint].key == newRecord->key) { return 1; } //Chave ja existe, retorna erro

    int status;
    if(!newPage->bPage->isLeaf) { //Caso não seja folha

        newPageInfo *searchPage = getPageFromBTreeFile(newPage->bPage->childs[insertPoint]);
        status = _bTreeInsert(newRecord, searchPage, promoted);
        //Chama a recursão na subárvore adequada

        if(*promoted == NULL) { return 0; } //Inserção realizada com sucesso

        //Inserção com overflow, necessidade de inserir promoted
        //TODO caso esta página seja a raíz, é necessário overflow
        int insertPoint = pageBinarySearch((*promoted)->rec->key, newPage->bPage->records, 0, newPage->bPage->numRecords);
        bTreeInsertIntoPage((*promoted)->rec, promoted, newPage, insertPoint);
        
        printf("Key %d RRN %ld\nC0 %ld C1 %ld\n", (*promoted)->rec->key, (*promoted)->rec->RRN, (*promoted)->childs[0], (*promoted)->childs[1]);
        
        *promoted = NULL; //É necessário atualizar para null, se não todo overflow atualiza a raíz
        return status;
    }

    else {
        bTreeInsertIntoPage(newRecord, promoted, newPage, insertPoint);
    }

    return 0;
}


// *Função para inserir dados em nova página em caso de overflow
void insertPageData(bTreePage *bPage, bTreePage *createdPage, long startingPosition) {
    
    createdPage->numRecords = bPage->numRecords - startingPosition + 1;

    for(long i = 0; i < createdPage->numRecords - 1; i++) {
        createdPage->childs[i] = bPage->childs[i + startingPosition];
        createdPage->records[i].key = bPage->records[i + startingPosition].key;
        createdPage->records[i].RRN = bPage->records[i + startingPosition].RRN;
    }
}

// *Função para remover dados de uma página com overflow
void cleanPageData(bTreePage *bPage, long startingPosition) {
    
    bPage->numRecords = startingPosition;

    for(long i = startingPosition; i < MAXKEYS; i++) {
        bPage->childs[i] = -1;
        if(i != (MAXKEYS-1)) { 
            bPage->records[i].key = 0;
            bPage->records[i].RRN = -1;
        }
    }
}


// *Função para promover um record
promotedKey *promoteKey(record *rec, int LeftRRN, int RightRRN) {
    promotedKey *promoted = (promotedKey*)malloc(sizeof(promotedKey));
    promoted->childs[0] = LeftRRN;
    promoted->childs[1] = RightRRN;
    printf("fio %ld %ld\n", promoted->childs[0], promoted->childs[1]);
    promoted->rec = rec;
    return promoted;
}

// *Função para inserir uma nova chave em um nó
void bTreeInsertIntoPage(record *newRecord, promotedKey **promoted, newPageInfo *newPage, long insertPosition) {

    if(newPage->bPage->numRecords == MAXKEYS - 1) {

        //Nó está cheio, overflow
        newPageInfo *createdPage = (newPageInfo*)malloc(sizeof(newPageInfo));
        createdPage->bPage = createPage();
        FILE *fp = fopen(BTREEFILENAME, "r+");
        fseek(fp, 0, SEEK_END);
        createdPage->RRN = ftell(fp) / PAGESIZE;
        printf("rrn: %ld\n", createdPage->RRN);
        int insertPlace = 0;
        int maxKeysIsOdd = 0;
        long promotedIndex;

        if(MAXKEYS % 2 == 0) {
            if(insertPosition > MAXKEYS / 2 - 1) { 
                promotedIndex = MAXKEYS / 2 - 1; 
                insertPlace = 1;
            } else { 
                if(insertPosition == MAXKEYS / 2 - 1) { insertPlace = 0; } 
                else { insertPlace = -1; }
                promotedIndex = MAXKEYS / 2 - 2; 
            }
        }

        else {
            maxKeysIsOdd = 1;
            if(insertPosition > MAXKEYS / 2) { 
                promotedIndex = MAXKEYS / 2; 
                insertPlace = 1;
            } else { 
                if(insertPosition == MAXKEYS / 2) { insertPlace = 0; }
                else { insertPlace = -1; }
                promotedIndex = MAXKEYS / 2 - 1; 
            }
        }

        record *rec = (record*)malloc(sizeof(record));
        if(insertPlace == 0) { 
            rec->key = newRecord->key;
            rec->RRN = newRecord->RRN; 
        } else { 
            rec->key = newPage->bPage->records[promotedIndex].key;
            rec->RRN = newPage->bPage->records[promotedIndex].RRN; 
        }

        *promoted = promoteKey(rec, newPage->RRN, createdPage->RRN);
        insertPageData(newPage->bPage, createdPage->bPage, promotedIndex + 1);
        if(!insertPlace) { cleanPageData(newPage->bPage, promotedIndex + 1); }
        else { cleanPageData(newPage->bPage, promotedIndex); }
        

        if(insertPlace == 1) {
            insertPosition -= MAXKEYS / 2;
            if(maxKeysIsOdd) { insertPosition--; }

            //Criar função separada para os for
            for(int i = (MAXKEYS-2); i > insertPosition; i--) { 
                createdPage->bPage->records[i].key = createdPage->bPage->records[i-1].key;
                createdPage->bPage->records[i].RRN = createdPage->bPage->records[i-1].RRN;
            }
            createdPage->bPage->records[insertPosition].key = newRecord->key;
            createdPage->bPage->records[insertPosition].RRN = newRecord->RRN;
        }

        else if(insertPlace == -1) {
            for(int i = (MAXKEYS-2); i > insertPosition; i--) { 
                newPage->bPage->records[i].key = newPage->bPage->records[i-1].key;
                newPage->bPage->records[i].RRN = newPage->bPage->records[i-1].RRN;
            }
            newPage->bPage->records[insertPosition].key = newRecord->key;
            newPage->bPage->records[insertPosition].RRN = newRecord->RRN;
        }
        
        insertNodeInBTreeFile(createdPage, fp, createdPage->RRN);

    }

    else {
        if(*promoted != NULL) {
            //Precisa atualizar os filhos
            for(int i = (MAXKEYS - 1); i > insertPosition; i--) {
                if(i != MAXKEYS - 1) {
                    newPage->bPage->records[i].key = newPage->bPage->records[i-1].key;
                    newPage->bPage->records[i].RRN = newPage->bPage->records[i-1].RRN;
                }
                newPage->bPage->childs[i] = newPage->bPage->childs[i-1];
            }
            newPage->bPage->childs[insertPosition + 1] = (*promoted)->childs[1];
            newPage->bPage->childs[insertPosition] = (*promoted)->childs[0];
        }

        else {
            //Não precisa mexer nos filhos
            for(int i = (MAXKEYS - 2); i > insertPosition; i--) {
                newPage->bPage->records[i].key = newPage->bPage->records[i-1].key;
                newPage->bPage->records[i].RRN = newPage->bPage->records[i-1].RRN;
            }
        }

        newPage->bPage->records[insertPosition].key = newRecord->key;
        newPage->bPage->records[insertPosition].RRN = newRecord->RRN;
        newPage->bPage->numRecords++;
    }
}


// *Função para atualizar a raíz
// Problema: Por algum motivo da pau no segundo overflow
int headerUpdate(promotedKey *promoted, FILE* bFile) {

    printf("Atualizei header\n");
    newPageInfo *newPage = (newPageInfo*)malloc(sizeof(newPageInfo));
    newPage->bPage = createPage();
    fseek(bFile, sizeof(int), SEEK_SET);
    newPage->RRN = ftell(bFile);
    newPage->bPage->isLeaf = 0;
    //Aloca uma nova página

    bTreeInsertIntoPage(promoted->rec, &promoted, newPage, 0);
    // Insere o promoted no nó criado

    fseek(bFile, sizeof(int), SEEK_SET);
    fwrite(&newPage->RRN, PAGESIZE - sizeof(int), 1, bFile);
    fflush(bFile);
    //Atualiza o cabeçalho do arquivo da bTree
    
    insertNodeInBTreeFile(newPage, bFile, newPage->RRN);
    //Insere nova página no arquivo da bTree

    return 0;
}


//Imprimir nó - Apenas para debugging
void printNode(bTreePage *bPage) {
    printf("Records: %d\n", bPage->numRecords);
    for(int i = 0; i < MAXKEYS; i++) {
        printf("CHILD: %ld ", bPage->childs[i]);
        
        if(i != (MAXKEYS-1)) { 
            printf("KEY: %d ", bPage->records[i].key);
            printf("RRN: %ld\n", bPage->records[i].RRN);
        } else { printf("\n"); }
    }
}
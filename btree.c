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
  
    if(records[middle].key == searchKey) { return -1; }
    if(middle == firstSearch && middle == lastSearch) { return middle; }

    if(records[middle].key < searchKey) { 
        return binarySearchForInsertion(searchKey, records, middle+1, lastSearch);
    }

    return binarySearchForInsertion(searchKey, records, firstSearch, middle);  
}


long bTreeSearch(int searchKey) {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    newPageInfo *newPage = getOrCreateRoot(bFile);

    int elementPosition = _bTreeSearch(newPage, searchKey);
    if(elementPosition == -1) { return -1; } //Não encontrou a chave
    return newPage->bPage->records[elementPosition].RRN; //Retorna o RRN da chave encontrada
}


int _bTreeSearch(newPageInfo *newPage, int searchKey) {
    long elementPosition = pageBinarySearch(searchKey, newPage->bPage->records, 0, newPage->bPage->numRecords);

    if(newPage->bPage->records[elementPosition].key == searchKey) { return elementPosition; }

    if(!newPage->bPage->isLeaf) { //Caso não seja folha
        if(searchKey < newPage->bPage->records[elementPosition].key) {
            //Se chave de busca < registro em insertPoint, chama recursão no filho a esquerda
            newPageInfo *searchPage = getPageFromBTreeFile(newPage->bPage->records[elementPosition].RRN);
            return _bTreeSearch(searchPage, searchKey);
        } else {
            //Se chave de busca > registro em insertPoint, chama recursão no filho a direita
            newPageInfo *searchPage = getPageFromBTreeFile(newPage->bPage->records[elementPosition+1].RRN);
            return _bTreeSearch(searchPage, searchKey);
        }
    }

    else { return -1; } //Chave não encontrada
}


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

void printPromoted(promotedKey *p) {
    printf("Key: %d RRN: %ld\n", p->rec->key, p->rec->RRN);
    printf("c1: %ld c2 %ld\n\n", p->childs[0], p->childs[1]);
}


int bTreeInsert(record *newRecord) {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    newPageInfo *newPage = getOrCreateRoot(bFile);
     
    promotedKey *promoted = NULL;

    int error = _bTreeInsert(newRecord, newPage, &promoted);

    if(error == 1) { return 1; }
    if(promoted != NULL) { headerUpdate(promoted, bFile); }

    insertNodeInBTreeFile(newPage, bFile, newPage->RRN); //Vai ser necessário mudar no futuro

    fclose(bFile);
    free(newPage);
    return 0;
}


int _bTreeInsert(record *newRecord, newPageInfo *newPage, promotedKey **promoted) {

    int insertPoint = binarySearchForInsertion(newRecord->key, newPage->bPage->records, 0, newPage->bPage->numRecords);
    if(insertPoint == -1) { return 1; } //Chave foi encontrada

    if(!newPage->bPage->isLeaf) { //Caso não seja folha
        if(newRecord->key > newPage->bPage->records[MAXKEYS-1].key && newPage->bPage->numRecords == MAXKEYS - 1) {
            newPageInfo *searchPage = getPageFromBTreeFile(newPage->bPage->childs[insertPoint]);
            return _bTreeInsert(newRecord, searchPage, promoted);
            //Esse nó tá cheio e minha chave é maior que o último, preciso descer em isnertPoint+1
            //TODO ver se é realmente necessário
        } else {
            newPageInfo *searchPage = getPageFromBTreeFile(newPage->bPage->childs[insertPoint]);
            return _bTreeInsert(newRecord, searchPage, promoted);
            //Chama a recursão na subárvore adequada
        }

        if(*promoted == NULL) { return 0; } //Consegui inserir
        printf("Preciso inserir promotedkey\n");
        //Preciso inserir promotedkey
    }

    else {
        *promoted = bTreeInsertIntoPage(newRecord, *promoted, newPage, insertPoint);
    }

    return 0;
}


// *Função para inserir dados em nova página em caso de overflow
void insertPageData(bTreePage *bPage, bTreePage *createdPage, long startingPosition) {
    createdPage->numRecords = bPage->numRecords - startingPosition;

    for(long i = 0; i < createdPage->numRecords; i++) {
        createdPage->childs[i] = bPage->childs[i + startingPosition];
        createdPage->records[i].key = bPage->records[i + startingPosition].key;
        createdPage->records[i].RRN = bPage->records[i + startingPosition].RRN;
    }
}

// *Função para remover dados de uma página com overflow
void cleanPageData(bTreePage *bPage, long startingPosition) {
    
    if(MAXKEYS % 2 == 0) { bPage->numRecords = startingPosition; }
    else { bPage->numRecords = startingPosition + 1; }

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
    promoted->rec = rec;
    return promoted;
}

// *Função para inserir uma nova chave em um nó
promotedKey *bTreeInsertIntoPage(record *newRecord, promotedKey *promoted, newPageInfo *newPage, long insertPosition) {

    if(newPage->bPage->numRecords == MAXKEYS - 1) {

        //Nó está cheio, overflow
        newPageInfo *createdPage = (newPageInfo*)malloc(sizeof(newPageInfo));
        createdPage->bPage = createPage();
        int promotedIsNewRecord = 0;
        int maxKeysIsOdd = 0;
        long promotedIndex;

        if(MAXKEYS % 2 == 0) {
            if(insertPosition > MAXKEYS / 2 - 1) { 
                promotedIndex = MAXKEYS / 2 - 1; 
                promotedIsNewRecord = 1;
            }
            else { 
                if(insertPosition == MAXKEYS / 2 - 1) { promotedIsNewRecord = 0; }
                else { promotedIsNewRecord = -1; }
                promotedIndex = MAXKEYS / 2 - 2; 
            }
        }

        else {
            maxKeysIsOdd = 1;
            if(insertPosition > MAXKEYS / 2) { 
                promotedIndex = MAXKEYS / 2; 
                promotedIsNewRecord = 1;
            }
            else { 
                if(insertPosition == MAXKEYS / 2) { promotedIsNewRecord = 0; }
                else { promotedIsNewRecord = -1; }
                promotedIndex = MAXKEYS / 2 - 1; 
            }
        }

        record *rec;
        if(!promotedIsNewRecord) { rec = newRecord; }
        else { rec = &newPage->bPage->records[promotedIndex]; }
        promoted = promoteKey(rec, 1, 2);

        insertPageData(newPage->bPage, createdPage->bPage, promotedIndex + 1);
        if(!promotedIsNewRecord) { cleanPageData(newPage->bPage, promotedIndex + 1); }
        else { cleanPageData(newPage->bPage, promotedIndex); }


        if(promotedIsNewRecord == 1) {
            insertPosition -= MAXKEYS / 2;
            if(maxKeysIsOdd) { insertPosition--; }

            for(int i = (MAXKEYS-2); i > insertPosition; i--) { 
                createdPage->bPage->records[i].key = createdPage->bPage->records[i-1].key;
                createdPage->bPage->records[i].RRN = createdPage->bPage->records[i-1].RRN;
            }
            createdPage->bPage->records[insertPosition].key = newRecord->key;
            createdPage->bPage->records[insertPosition].RRN = newRecord->RRN;
        }

        else if(promotedIsNewRecord == -1) {
            for(int i = (MAXKEYS-2); i > insertPosition; i--) { 
                newPage->bPage->records[i].key = newPage->bPage->records[i-1].key;
                newPage->bPage->records[i].RRN = newPage->bPage->records[i-1].RRN;
            }
            newPage->bPage->records[insertPosition].key = newRecord->key;
            newPage->bPage->records[insertPosition].RRN = newRecord->RRN;
        }

        FILE *fp = fopen(BTREEFILENAME, "r+");
        insertNodeInBTreeFile(createdPage, fp, -1);

    }

    else {
        //Falta atualizar os filhos!!!
        for(int i = (MAXKEYS-2); i > insertPosition; i--) { 
            newPage->bPage->records[i].key = newPage->bPage->records[i-1].key;
            newPage->bPage->records[i].RRN = newPage->bPage->records[i-1].RRN;
        }
        newPage->bPage->records[insertPosition].key = newRecord->key;
        newPage->bPage->records[insertPosition].RRN = newRecord->RRN;
        newPage->bPage->numRecords++;
    }

    printNode(newPage->bPage);

    return promoted;
}


int headerUpdate(promotedKey *promoted, FILE* bFile) {
    newPageInfo *newPage = (newPageInfo*)malloc(sizeof(newPageInfo));
    newPage->bPage = createPage();
    fseek(bFile, sizeof(int), SEEK_END);
    newPage->RRN = ftell(bFile);
    newPage->bPage->isLeaf = 0;

    bTreeInsertIntoPage(promoted->rec, promoted, newPage, 0);
    newPage->bPage->childs[0] = promoted->childs[0];
    newPage->bPage->childs[1] = promoted->childs[1];

    fseek(bFile, sizeof(int), SEEK_SET);
    fwrite(&newPage->RRN, PAGESIZE - sizeof(int), 1, bFile);
    fflush(bFile);
    
    insertNodeInBTreeFile(newPage, bFile, newPage->RRN);

    return 0;
}


void bTreePrint(long RRN) {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    newPageInfo *newPage = getPageFromBTreeFile(RRN);
    printNode(newPage->bPage);
    free(newPage);
}

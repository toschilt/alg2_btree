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
bPageInfo *createRoot(FILE *bFile) {
    bPageInfo *bInfo = (bPageInfo*)malloc(sizeof(bPageInfo));
    bInfo->bPage = createPage();
    bInfo->RRN = 1;
    int created = -1;
    
    fseek(bFile, 0, SEEK_SET);
    fwrite(&created, sizeof(int), 1, bFile);
    fwrite(&bInfo->RRN, PAGESIZE - sizeof(int), 1, bFile);
    fflush(bFile);
    
    insertNodeInBTreeFile(bInfo, bFile, bInfo->RRN);
    return bInfo;
}


// *Função utilizada para coletar a raíz
bPageInfo *getOrCreateRoot(FILE *bFile) {
    int created, RRN;
    fseek(bFile, 0, SEEK_SET);
    fread(&created, sizeof(int), 1, bFile);

    if(created != -1) {
        bPageInfo *bInfo = createRoot(bFile); 
        return bInfo;
    } else {
        fread(&RRN, sizeof(int), 1, bFile);
        bPageInfo *bInfo = getPageFromBTreeFile(RRN);
        return bInfo;
    }
}

// *Função para busca binária de chave em nó da árvore
long pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch) {
    long middle = (firstSearch + lastSearch) / 2;

    if(records[middle].key == searchKey || (middle == firstSearch && middle == lastSearch)) { return middle; }

    if(records[middle].key < searchKey) {  return pageBinarySearch(searchKey, records, middle+1, lastSearch); }

    return pageBinarySearch(searchKey, records, firstSearch, middle); 
}


// *Função para inserir novo nó na árvore
long bTreeSearch(int searchKey) {
    FILE *bFile = fopen(BTREEFILENAME, "r+");
    bPageInfo *bInfo = getOrCreateRoot(bFile);

    return _bTreeSearch(bInfo, searchKey);
    //Abre a raiz e chama a função recursiva
}


long _bTreeSearch(bPageInfo *bInfo, int searchKey) {
    long insertPoint = pageBinarySearch(searchKey, bInfo->bPage->records, 0, bInfo->bPage->numRecords);
    //Realiza a busca binária para determinar a posição do elemento

    if(insertPoint < MAXKEYS - 1 && bInfo->bPage->records[insertPoint].key == searchKey) { 
        return bInfo->bPage->records[insertPoint].RRN; 
    }
    //Caso o elemento seja encontrado, retorna seu RRN

    if(!bInfo->bPage->isLeaf) { 
        //Caso não seja folha, chama recursão na página adequada
        bPageInfo *searchPage = getPageFromBTreeFile(bInfo->bPage->childs[insertPoint]);
        printNode(searchPage->bPage);
        return _bTreeSearch(searchPage, searchKey);
    }

    else { return -1; } 
    //É folha e não encontrou, chave não existe
}


// *Função para inserir novo nó na bTree
int bTreeInsert(record *newRecord) {

    FILE *bFile = fopen(BTREEFILENAME, "r+");
    bPageInfo *bInfo = getOrCreateRoot(bFile);
    promotedKey *promoted = NULL;
    //Coleta a raíz e cria um promotedKey, responsável por cuidar de overflows relacionados
    //à atualização de raiz

    int error = _bTreeInsert(newRecord, bInfo, &promoted);
    //Chama a função recursiva

    if(error == -1) { return error; }
    //Não foi possível inserir a chave

    if(promoted != NULL) { headerUpdate(promoted, bFile); }
    //Ao fim do processo, se promoted é não nulo, houve overflow na raíz, necessidade de atualizar
    //o cabeçalho do arquivo

    insertNodeInBTreeFile(bInfo, bFile, bInfo->RRN);
    //Atualiza a raíz. Útil caso tenha ocorrido alguma modificação nesta

    fclose(bFile);
    free(bInfo);
    return 0;
}


// *Função recursiva para inserir nova chave
int _bTreeInsert(record *newRecord, bPageInfo *bInfo, promotedKey **promoted) {

    int insertPoint = pageBinarySearch(newRecord->key, bInfo->bPage->records, 0, bInfo->bPage->numRecords);
    //Encontra o índice da chave na página

    if(insertPoint == MAXKEYS - 1 && bInfo->bPage->records[insertPoint].key == newRecord->key) { return -1; } //Chave ja existe, retorna erro
    //Verifica se o elemento já existe no nó

    int status;
    if(!bInfo->bPage->isLeaf) { //Caso não seja folha

        bPageInfo *searchPage = getPageFromBTreeFile(bInfo->bPage->childs[insertPoint]);
        status = _bTreeInsert(newRecord, searchPage, promoted);
        //Chama a recursão na subárvore adequada

        if(*promoted == NULL) { return 0; } //Inserção realizada com sucesso e sem overflow

        //Promoted diferente de nulo, inserção com overflow, necessidade de inserir promoted
        int insertPoint = pageBinarySearch((*promoted)->rec->key, bInfo->bPage->records, 0, bInfo->bPage->numRecords);
        bTreeInsertIntoPage((*promoted)->rec, promoted, bInfo, insertPoint);
        //Encontra a posição de inserção de promoted e insere na página
        
        return status;
    }

    //Não é folha e o elemento não existe, realiza a inserção
    else {
        bTreeInsertIntoPage(newRecord, promoted, bInfo, insertPoint);
    }

    return 0;
}


// *Função para inserir dados em nova página em caso de overflow
void insertPageData(bTreePage *bPage, bTreePage *createdPage, long startingPosition) {

    if(startingPosition >= MAXKEYS / 2 + 1) {
        createdPage->numRecords = startingPosition - 1;
        bPage->numRecords = MAXKEYS - startingPosition;
    } else {
        createdPage->numRecords = startingPosition;
        bPage->numRecords = MAXKEYS - startingPosition - 1;
    }


    //Copia informações do fim da página com overflow para a página nova
    printf("Starting insertpage: %d\n", createdPage->numRecords);
    for(long i = 0; i < startingPosition - 1; i++) {
        createdPage->childs[i] = bPage->childs[i + MAXKEYS - startingPosition];

        if(i != (MAXKEYS-1)) {
            createdPage->records[i].key = bPage->records[i + MAXKEYS - startingPosition].key;
            createdPage->records[i].RRN = bPage->records[i + MAXKEYS - startingPosition].RRN;
        }
    }
}

// *Função para remover dados de uma página com overflow
void cleanPageData(bTreePage *bPage, long startingPosition) {
    
    // bPage->numRecords = MAXKEYS - startingPosition - 2;

    //Apaga informações inseridas na nova página da página com overflow
    printf("Starting clean: %ld\n", startingPosition);
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
void bTreeInsertIntoPage(record *newRecord, promotedKey **promoted, bPageInfo *bInfo, long insertPosition) {

    if(bInfo->bPage->numRecords == MAXKEYS - 1) {

        //Nó está cheio, overflow
        bPageInfo *createdPage = (bPageInfo*)malloc(sizeof(bPageInfo));
        createdPage->bPage = createPage();
        FILE *fp = fopen(BTREEFILENAME, "r+");
        fseek(fp, 0, SEEK_END);
        createdPage->RRN = ftell(fp) / PAGESIZE;
        
        int newRecordInsertPlace = 0;
        //0 - Nova chave precisa ser promovida, 1 - Chave inserida na direita, 2 - Chave inserida na esquerda

        int maxKeysIsOdd = 0;
        long promotedIndex;

        if(MAXKEYS % 2 == 0) {
            if(insertPosition > MAXKEYS / 2 - 1) {
                //Preciso inserir o elemento na direita, OK
                promotedIndex = MAXKEYS / 2 - 1; 
                newRecordInsertPlace = 1;
            } else { 
                if(insertPosition == MAXKEYS / 2 - 1) { 
                    //Novo record corresponde a chave promovida
                    newRecordInsertPlace = 0; 
                } 
                else { newRecordInsertPlace = -1; } //Preciso inserir na esquerda
                promotedIndex = MAXKEYS / 2 - 2; 
            }
        }
        else {
            maxKeysIsOdd = 1;
            if(insertPosition > MAXKEYS / 2) { 
                promotedIndex = MAXKEYS / 2; 
                newRecordInsertPlace = 1;
            } else { 
                if(insertPosition == MAXKEYS / 2) { newRecordInsertPlace = 0; }
                else { newRecordInsertPlace = -1; }
                promotedIndex = MAXKEYS / 2 - 1; 
            }
        }

        record *rec = (record*)malloc(sizeof(record));
        if(newRecordInsertPlace == 0) { 
            rec->key = newRecord->key;
            rec->RRN = newRecord->RRN; 
        } else { 
            rec->key = bInfo->bPage->records[promotedIndex].key;
            rec->RRN = bInfo->bPage->records[promotedIndex].RRN; 
        }

        *promoted = promoteKey(rec, bInfo->RRN, createdPage->RRN);

        printf("Insert: %d\n", newRecordInsertPlace);
        
        if(newRecordInsertPlace == 1)
            insertPageData(bInfo->bPage, createdPage->bPage, promotedIndex);
        else if(newRecordInsertPlace == 0)
            insertPageData(bInfo->bPage, createdPage->bPage, promotedIndex + 2);
        else
            insertPageData(bInfo->bPage, createdPage->bPage, promotedIndex + 2);
            
        if(newRecordInsertPlace == 1)
            cleanPageData(bInfo->bPage, promotedIndex);
        else if(newRecordInsertPlace == 0)
            cleanPageData(bInfo->bPage, promotedIndex + 1);
        else
            cleanPageData(bInfo->bPage, promotedIndex);
        

        if(newRecordInsertPlace == 1) {
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

        else if(newRecordInsertPlace == -1) {
            for(int i = (MAXKEYS-2); i > insertPosition; i--) { 
                bInfo->bPage->records[i].key = bInfo->bPage->records[i-1].key;
                bInfo->bPage->records[i].RRN = bInfo->bPage->records[i-1].RRN;
            }
            bInfo->bPage->records[insertPosition].key = newRecord->key;
            bInfo->bPage->records[insertPosition].RRN = newRecord->RRN;
        }
        
        insertNodeInBTreeFile(createdPage, fp, createdPage->RRN);

    }
    else {
        if(*promoted != NULL) {
            //Precisa atualizar os filhos
            for(int i = (MAXKEYS - 1); i > insertPosition; i--) {
                if(i != MAXKEYS - 1) {
                    bInfo->bPage->records[i].key = bInfo->bPage->records[i-1].key;
                    bInfo->bPage->records[i].RRN = bInfo->bPage->records[i-1].RRN;
                }
                bInfo->bPage->childs[i] = bInfo->bPage->childs[i-1];
            }
            bInfo->bPage->childs[insertPosition + 1] = (*promoted)->childs[1];
            bInfo->bPage->childs[insertPosition] = (*promoted)->childs[0];
        }
        else {
            //Não precisa mexer nos filhos
            for(int i = (MAXKEYS - 2); i > insertPosition; i--) {
                bInfo->bPage->records[i].key = bInfo->bPage->records[i-1].key;
                bInfo->bPage->records[i].RRN = bInfo->bPage->records[i-1].RRN;
            }
        }

        bInfo->bPage->records[insertPosition].key = newRecord->key;
        bInfo->bPage->records[insertPosition].RRN = newRecord->RRN;
        bInfo->bPage->numRecords++;

        *promoted = NULL;
    }
}


// *Função para atualizar a raíz
// Problema: Por algum motivo da pau no segundo overflow
int headerUpdate(promotedKey *promoted, FILE* bFile) {

    printf("Atualizei header\n");
    bPageInfo *bInfo = (bPageInfo*)malloc(sizeof(bPageInfo));
    bInfo->bPage = createPage();
    fseek(bFile, 0, SEEK_END);
    bInfo->RRN = ftell(bFile)/PAGESIZE;
    bInfo->bPage->isLeaf = 0;
    //Aloca uma nova página

    bTreeInsertIntoPage(promoted->rec, &promoted, bInfo, 0);
    // Insere o promoted no nó criado

    fseek(bFile, sizeof(int), SEEK_SET);
    fwrite(&bInfo->RRN, PAGESIZE - sizeof(int), 1, bFile);
    fflush(bFile);
    //Atualiza o cabeçalho do arquivo da bTree
    
    insertNodeInBTreeFile(bInfo, bFile, bInfo->RRN);
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
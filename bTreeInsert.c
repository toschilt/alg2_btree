#include "bTreeInsert.h"
#include "streamHandler.h"


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
    //Não foi possível inserir a chave, registro já existe

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

    //Verifica se o elemento já existe no nó
    if(insertPoint < MAXKEYS - 1 && bInfo->bPage->records[insertPoint].key == newRecord->key) { 
        //Chave ja existe, retorna erro
        return -1;
    } 

    int status;
    if(!bInfo->bPage->isLeaf) { //Caso não seja folha

        bPageInfo *searchPage = getPageFromBTreeFile(bInfo->bPage->childs[insertPoint]);
        status = _bTreeInsert(newRecord, searchPage, promoted);
        //Chama a recursão na subárvore adequada

        if(status == -1) { return -1; } //Ao voltar da recursão, checka se o registro existe
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
void insertPageData(bTreePage *bPage, bTreePage *createdPage, long *childsArray, long startingPosition) {

    //Seta a quantidade de elementos por página de acordo com a posição de inserção
    if(startingPosition >= MAXKEYS / 2 + 1) {
        createdPage->numRecords = startingPosition - 1;
        bPage->numRecords = MAXKEYS - startingPosition;
    } else {
        createdPage->numRecords = startingPosition;
        bPage->numRecords = MAXKEYS - startingPosition - 1;
    }

    //Copia informações do fim da página com overflow para a página nova
    for(long i = 0; i < startingPosition - 1; i++) {
        createdPage->records[i].key = bPage->records[i + MAXKEYS - startingPosition].key;
        createdPage->records[i].RRN = bPage->records[i + MAXKEYS - startingPosition].RRN;
    }

    if(childsArray != NULL) {
        for(int i = 0; i < MAXKEYS; i++) {

            if(i <= MAXKEYS - bPage->numRecords - 1) { 
                createdPage->childs[i] = childsArray[i + MAXKEYS - bPage->numRecords]; 
            } else { 
                createdPage->childs[i] = -1; 
            }
        }
    }
}

// *Função para remover dados de uma página com overflow
void cleanPageData(bTreePage *bPage, long *childsArray, long startingPosition) {
    
    //Apaga informações inseridas na nova página da página com overflow
    for(long i = startingPosition; i < MAXKEYS - 1; i++) {
        bPage->records[i].key = 0;
        bPage->records[i].RRN = -1;
    }

    if(childsArray != NULL) {
        for(int i = 0; i < MAXKEYS; i++) {
            
            if(i <= bPage->numRecords) { 
                bPage->childs[i] = childsArray[i]; 
            } else { 
                bPage->childs[i] = -1; 
            }
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

// *Função para criar um vetor de MAXKEYS + 1 posições, contendo a informação
// *de todos os filhos que precisarão ser alocados depois
long *createChildsArray(bPageInfo *bInfo, promotedKey *promoted) {
    static long childsArray[MAXKEYS + 1];
    int found = 0;

    for(int i = 0; i < MAXKEYS + 1; i++) {
        
        childsArray[i + found] = bInfo->bPage->childs[i];

        if(bInfo->bPage->childs[i] == promoted->childs[0]) {
            found = 1;
            childsArray[i+1] = promoted->childs[1];
        }
    }

    return childsArray;
}


// *Função para inserir uma nova chave em um nó
void bTreeInsertIntoPage(record *newRecord, promotedKey **promoted, bPageInfo *bInfo, long insertPosition) {

    if(bInfo->bPage->numRecords == MAXKEYS - 1) {

        //Nó está cheio, overflow
        bPageInfo *createdPage = (bPageInfo*)malloc(sizeof(bPageInfo));
        createdPage->bPage = createPage();
        createdPage->bPage->isLeaf = bInfo->bPage->isLeaf;

        //TODO remover a abertura de arquivo daqui
        FILE *fp = fopen(BTREEFILENAME, "r+");
        fseek(fp, 0, SEEK_END);
        createdPage->RRN = ftell(fp) / PAGESIZE;
        
        int newRecordInsertPlace = 0;
        //  0 - Chave inserida precisa ser promovida 
        //  1 - Chave inserida na direita 
        // -1 - Chave inserida na esquerda

        int maxKeysIsOdd = 0;
        long promotedIndex;
        
        long *childsArray = NULL;

        // printf("Imprimindo nó\n");
        // printNode(bInfo);

        if(*promoted != NULL) {
            //Cria um vetor com filhos de bInfo e promoted que será utilizado no fim
            childsArray = createChildsArray(bInfo, *promoted);
        }

        if(MAXKEYS % 2 == 0) {
            if(insertPosition > MAXKEYS / 2 - 1) {
                //É necessário inserir o elemento na direita
                promotedIndex = MAXKEYS / 2 - 1; 
                newRecordInsertPlace = 1;
            } else { 
                if(insertPosition == MAXKEYS / 2 - 1) { 
                    //Novo record corresponde a chave promovida
                    newRecordInsertPlace = 0; 
                } else { 
                    //É necessário inserir na esquerda
                    newRecordInsertPlace = -1; 
                } 

                promotedIndex = MAXKEYS / 2 - 2; 
            }
        }
        else {
            //Idem ao caso acima, mas para MAXKEYS ímpar
            maxKeysIsOdd = 1;
            if(insertPosition > MAXKEYS / 2) { 
                promotedIndex = MAXKEYS / 2; 
                newRecordInsertPlace = 1;
            
            } else {
                if(insertPosition == MAXKEYS / 2) { 
                    newRecordInsertPlace = 0; 
                } else { 
                    newRecordInsertPlace = -1; 
                }

                promotedIndex = MAXKEYS / 2 - 1; 
            }
        }


        //Salva as informações do record a ser promovido em rec
        record *rec = (record*)malloc(sizeof(record));
        if(newRecordInsertPlace == 0) {
            rec->key = newRecord->key;
            rec->RRN = newRecord->RRN;
        } else {
            rec->key = bInfo->bPage->records[promotedIndex].key;
            rec->RRN = bInfo->bPage->records[promotedIndex].RRN; 
        }

        //Promove rec
        *promoted = promoteKey(rec, bInfo->RRN, createdPage->RRN);
        // printf("Promted:\n");
        // printPromoted(*promoted);
        
        //Atualização das páginas, adicionar elementos na nova página e remover da antiga
        if(newRecordInsertPlace == 1)
            insertPageData(bInfo->bPage, createdPage->bPage, childsArray, promotedIndex);
        
        else if(newRecordInsertPlace == 0)
            insertPageData(bInfo->bPage, createdPage->bPage, childsArray, promotedIndex + 2);
        
        else
            insertPageData(bInfo->bPage, createdPage->bPage, childsArray, promotedIndex + 2);
            
        
        if(newRecordInsertPlace == 1)
            cleanPageData(bInfo->bPage, childsArray, promotedIndex);
        
        else if(newRecordInsertPlace == 0)
            cleanPageData(bInfo->bPage, childsArray, promotedIndex + 1);
        
        else
            cleanPageData(bInfo->bPage, childsArray, promotedIndex);
        

        // printf("\n\nImprimindo nós após alteração\n");
        // printf("Filho 1:\n");
        // printNode(bInfo);
        // printf("Filho 2\n");
        // printNode(createdPage);
        
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

        // printf("\n\nImprimindo nós após alteração\n");
        // printf("Filho 1:\n");
        // printNode(bInfo);
        // printf("Filho 2\n");
        // printNode(createdPage);

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
int headerUpdate(promotedKey *promoted, FILE* bFile) {

    // printf("\nAtualizei header\n");
    bPageInfo *bInfo = (bPageInfo*)malloc(sizeof(bPageInfo));
    bInfo->bPage = createPage();
    fseek(bFile, 0, SEEK_END);
    //Vai para o início do arquivo e cria uma nova página

    bInfo->RRN = ftell(bFile) / PAGESIZE;
    bInfo->bPage->isLeaf = 0;
    //Aloca uma nova página

    bTreeInsertIntoPage(promoted->rec, &promoted, bInfo, 0);
    //Insere o promoted no nó criado

    fseek(bFile, sizeof(int), SEEK_SET);
    fwrite(&bInfo->RRN, PAGESIZE - sizeof(int), 1, bFile);
    fflush(bFile);
    //Atualiza o cabeçalho do arquivo da bTree
    
    insertNodeInBTreeFile(bInfo, bFile, bInfo->RRN);

    bInfo = getOrCreateRoot(bFile);
    //Insere nova página no arquivo da bTree

    return 0;
}
#include "bTreeUtils.h"
#include "streamHandler.h"

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


// *Imprimir nó - Apenas para debugging
void printNode(bPageInfo *bInfo) {
    printf("Records: %d - isLeaf: %d - RRN: %ld\n\n", bInfo->bPage->numRecords, bInfo->bPage->isLeaf, bInfo->RRN);
    for(int i = 0; i < MAXKEYS; i++) {
        printf("CHILD: %ld ", bInfo->bPage->childs[i]);
        
        if(i != (MAXKEYS - 1)) { 
            printf("KEY: %d ", bInfo->bPage->records[i].key);
            printf("RRN: %ld\n", bInfo->bPage->records[i].RRN);
        } else { printf("\n\n"); }
    }
}
#include "bTreeSearch.h"
#include "streamHandler.h"


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
#include "btree.h"
#include "streamHandler.h"


bTreePage *createTree(FILE *bTreeFile) {

}

bTreePage *getOrCreateRoot(FILE *bTreeFile) {

}


bTreePage *getPage(long RRN, FILE *bTreeFilePointer) {

}


long getTreeHeader(FILE *bTreeFile) {

}


void writeTreeHeader(FILE *bTreeFile, long rootRRN) {

}


promotedKey *insertIntoNode(bTreePage *page, promotedKey *newKey, FILE *bTreeFile) {

}



bTreePage *searchPositionOnPageAndInsert(bTreePage *page, promotedKey *newKey) {

}


bTreePage *splitAndCreateNewNode(bTreePage **page) {

}


promotedKey *extractPromotedKey(bTreePage *page) {

}


promotedKey *split(bTreePage *page, FILE *bTreeFilePointer, promotedKey *newKey) {

}


bTreePage *createNodeWithPromotedKey(promotedKey *promoKey) {

}


Errors setNodeAsRoot(bTreePage *page, FILE *bTreeFilePointer) {

}



bTreePage *readPageFromFile(FILE *bTreeFile) {

}


Errors writePageIntoFile(long rrn, bTreePage *page, FILE *dataFilePointer) {

}


promotedKey *_bTreeInsert(bTreePage *node, promotedKey *key, FILE *bTreeFilePointer) {

}



Errors bTreeInsert(PrimaryIndex *newRecord, bTreePage *root, FILE *bTreeFilePointer) {

}


long bTreeSelect(bTreePage *node, int key, FILE* bTreeFilePointer) {

}


// Função antiga para inserção:
// Errors bTreeInsert(record *record, bTreePage *root, FILE *bTreeFile)

/* Carrega a raíz do arquivo da bTree
    
SITUAÇÕES DE INSERÇÃO

ÁRVORE VAZIA
- Criação e preenchimento do nó raíz
- Primeira chave: Criação do nó raíz
- Demais chaves: Inserção até a capacidade limite do nó
* Chaves ordenadas

- Nó raíz = nó folha
- Ponteiros: -1


OVERFLOW NO NÓ RAÍZ
- Particionamento do nó (split)
- Split 1-to-2
- As chaves são distribuídas uniformemente nos dois nós
- As duas páginas novas possuem metade da capacidade preenchida

- Criação de uma nova raíz
- A nova raíz é constituída de um elemento

- A primeira chave do novo nó após particionamento é promovida para o nó raíz
- O pontiero da esqueda do novo nó raíz aponta para a página inicial
- O ponteiro da direita do novo nó raíz aponta para a página recêm-criada


INSERÇÃO EM NÓS FOLHA
- Pesquisa
- A árvore é percorrida até encontrar o nó filha no qual a nova chave será inserida
- Páginas são lidas para a memória principal

    INSERÇÃO SEM OVERFLOW
    - Ordenação das chaves após a inserção

    INSERÇÃO COM OVERFLOW
    - Particionamento do nó (split)
    - Criação de um novo nó
    - Nó original = nó original + novo nó
    - Distribuição unifomre das chaves nos dois nós

    - Promoção
    - A escolha da primeira chave do novo nó como chave separadora no nó pai
    - Reordenação e ajuste do nó pai para apontar para o novo nó
    - Propagação de overflow
*/


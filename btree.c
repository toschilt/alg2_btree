#include "btree.h"
#include "streamHandler.h"


bTreePage *createRoot(FILE *bTreeFile) {
    //Cria uma nova árvore
    //Salva as informações do nó raíz no arquivo
}


bTreePage *getRootBTree(FILE *bTreeFile) {
    /* Inicialização da árvore
     * Verifica se há uma raíz no arquivo da bTree
     * Caso não haja, chama a função para criação de uma nova raíz
     * Caso haja, coleta a raíz e retorna a página
    */
}


Errors bTreeInsert(record *record, bTreePage *root, FILE *bTreeFile) {
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
}


Errors bTreeUpdate(record *record, bTreePage *root, FILE *bTreeFile) {
    /* Coleta o nó raiz
     * Chama a função de busca
     * Caso seja encontrado, re-escreve as informações no nó e salva o arquivo
     * Caso não seja encontrado retorna erro
    */ 
}


long bTreeSearch(bTreePage *root, int key, FILE *bTreeFile) {
    /* Coleta o nó raíz
     * Busca pela chave na página
     * Caso não encontre, busca na subárvore apropriada
     * Recursão até encontrar a chave 
    */
}
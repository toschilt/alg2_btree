#include "btree.h"
#include "streamHandler.h"


bTreePage *createRoot(FILE *bFile) {

}


bTreePage *getOrCreateRoot(FILE *bFile) {

}



long bTreeSearch(int key) {

}


bTreePage *_bTreeSearch(bTreePage *bPage, int searchKey) {

}



record *pageBinarySearch(int searchKey, record *records, long firstSearch, long lastSearch) {

}


int bTreeInsert(record *newRecord) {

}


int *_bTreeInsert(int key, bTreePage *bPage, promotedKey **promoted) {

}



promotedKey *bTreeInsertIntoPage(promotedKey *promoted, bTreePage *bPage, long insertPosition) {

}


long binarySearchForInsertion(int searchKey, record *records, long firstSearch, long lastSearch) {

}


int headerUpdate(promotedKey *promoted) {

}


/* FUNÇÕES

-- BUSCA -- 
bTreeSearch(chave);
abrir o arquivo
getorcreateroot
caso necessário, createroot

função recursiva
bTreePage *_bTreeSearch(pagina p buscar, chave)
- Verifica se chave está em pagina p buscar com bb
    - Caso esteja, retorna pagina p buscar
    
    - Caso contrário:
    - Verifica se é folha
        - Caso seja, retorna nulo
        
        - Caso contrário
        - chama recursão em pagina adequada

se _bTreePage retorna nulo, nao encontrou
caso contrário
    - varre a página e retorna o rrn do arquivo de dados

retorna o RRN


getOrCreateRoot(arquivo)
- Abrir o arquivo
    - Se o arquivo tiver vazio, chama createRoot
    - Pega o RRN do cabeçalho
    - abre e retorna a página raiz


createRoot(arquivo)
- Cria o header
- Aloca uma página e insere no arquivo da btree
- retorna o ponteiro da pagina alocada


buscabinaria(key, vetor de registros, começo, fim)
verifica se pagina > local de busca / 2
    caso seja, chama bb em local de busca da direita
    return buscabinaria(key, vetor, final / 2, final)
    caso nao seja, chama bb em local de busca da esquerda
retorna record



-- INSERIR --
bTreeInsert(record);
abrir o arquivo
getorcreateroot
caso necessário, createroot
Chama _bTreeInsert
Checka se promotedKey é nulo
    Caso seja, sucesso :)
    
    Caso não seja,
    - Necessidade de atualizar o header
    headerUpdate(promotedKey)


Erro *_bTreeInsert(pagina p buscar, chave, **promotedKey)

- Chama bin search para verificar se o nó existe
    - Caso ache
        - retorna ConseguiInserir = 0

- Verifica se é folha
    - Se sim,
        - Função para fazer a inserção
        *promotedKey = bTreeInsertIntoPage(promotedKey, pagina, posição de inserção)
        return
            

    - Se não,
        - Acha página certa
        - Chama recursão na pagina certa

        - Checka se promotedKey é nulo
            - Se for
                - Retorna ConseguiInserir = 1
            - Se não for
                *promotedKey = bTreeInsertIntoPage(promotedKey, pagina, posição de inserção)



binarySearchForInsertion(key, vetor, srart, finish)
    - Caso encontre o elemento, retorna -1 (erro)
    - Caso contrário, retorna a posição de isnerção


promotedKey *bTreeInsertIntoPage(promotedKey, pagina, posição de inserção)
- Checka se página está cheia
    - Caso não
    Insere na posição
    Insere os ponteiros
    retorna NULL

    - Caso sim
    Cria novo nó
    Verifico onde vou inserir
        Se MAXKEYS for PAR
        Se for antes da metade:
        Pego a chave do meio -1 e promovo
        
        Se for depois da metade
        Pega a chave do meio e promovo

        Se MAXKEYS for IMPAR
        Se for antes da metade:
        Pego a chave do meio e promovo
        
        Se for depois da metade
        Pega a chave do meio + 1 e promovo

    Crio um novo nó com o conteúdo da direita



headerUpdate(promotedKey)
- Atualiza o header do arquivo p rrn de promotedKey




-- ATUALIZAR --



*/



//-----------------------------------------------------------------------------//



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





#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdio.h>
#include <stdlib.h>

//TAMANHO DA PÁGINA: 4096 Bytes
#define PAGESIZE 4096
#define MAXKEYS 204
// #define QUANTIDADE DE REGISTRO

typedef struct {
    int success;
    //Estrutura para armazenar possíveis erros (Ideia tirada do PDF do trabalho)
} Errors;

typedef struct {
    int key;
    long RRN;
} record;

typedef struct {
    record *records;
    long *childs;
    int numRecords;
    char isLeaf;
} bTreePage;

typedef struct {
    int key;
    long RRN;
    long childs[2];
} promotedKey;

typedef struct {
    //Sei la pra que que serve esse role
} PrimaryIndex;


/* Cria uma nova árvore
 * Salva as informações no nó raíz do arquivo
*/
bTreePage *createTree(FILE *bTreeFile);

/* Inicialização da árvore
 * Verifica se há uma raíz no arquivo da bTree
 * Caso não haja, chama a função createRoot
 * Caso haja, coleta a raíz e retorna a página
*/
bTreePage *getOrCreateRoot(FILE *bTreeFile);

/* Recupera uma página baseado no RRN
 * Procura e carrega seus dados
*/
bTreePage *getPage(long RRN, FILE *bTreeFilePointer);

/* Carrega o cabeçalho da árvore,
 * que está no início do arquivo.
*/
long getTreeHeader(FILE *bTreeFile);

/* 
 * Escreve o header no arquivo da bTree
*/
void writeTreeHeader(FILE *bTreeFile, long rootRRN);

/* Procura local para inserir nova chave na página
 * Se não couber, splitta ele e escreve dados na página
*/
promotedKey *insertIntoNode(bTreePage *page, promotedKey *newKey, FILE *bTreeFile);


/* Encontra a posição para inserir a chave
 * Se não existir espaço, precisa criar uma nova página
 * Salvar dados da nova chave na página
*/
bTreePage *searchPositionOnPageAndInsert(bTreePage *page, promotedKey *newKey);

/* Encontra a posição no meio das chaves 
 * Cria espaço pra nova página
 * Copia metade das chaves para página nova
 * Limpa elas do nó antigo
 * Autaliza os filhos de ambas as páginas
 * Atualiza o número de chaves de ambas
*/
bTreePage *splitAndCreateNewNode(bTreePage **page);

/* Aloca espaço para chave
 * Tira ela da página
 * Atualiza dados da página (filho, número de chaves, etc.)
*/
promotedKey *extractPromotedKey(bTreePage *page);

/* Divide a página, cria o novo nó
 * Extrai a chave promovida e atualiza os filhos da chave
 * Escreve a página nova e a que foi dividia (com suas atualizações) no arquivo
*/
promotedKey *split(bTreePage *page, FILE *bTreeFilePointer, promotedKey *newKey);

/* Se promoção cria estrutura para nova raíz
 * Aloca espaço para ela
 * Salva dados da chave promovida na nova raíz
 * Atualiza os filhos deste novo nó/página
 * Pode reusar função que cria página nova e adicionar somente especificidades
*/
bTreePage *createNodeWithPromotedKey(promotedKey *promoKey);

/* Escreve página nova e atualiza o cabeçalho para conter ela como raíz
 * Deveria ser chamada junto com criação de novo nó quando promoção cria uma nova raíz
*/
Errors setNodeAsRoot(bTreePage *page, FILE *bTreeFilePointer);


/* Aloca espaço para carregar página
 * Lê dados da página da BTree
 * Retorna a página com as informações obtidas ou um ponteiro nulo
*/
bTreePage *readPageFromFile(FILE *bTreeFile);

/* Verifica se está tudo ok com os dados
 * Encontra local para escrita baseado no RRN
 * Escreve dados
 * Atualiza valor de espaço livre na página
*/
Errors writePageIntoFile(long rrn, bTreePage *page, FILE *dataFilePointer);

/* Inserção recursiva
 * Se no nó a ser inserido, a chave é folha, tenta inserir
 * Caso a inserção crie uma promoção, precisa retornar a chave promovida para a recursão
 * Se não for nó folha, procura qual sub-árvore seguir para inserir numa folha
 * Encontrar a posição correta e descer para filho à esquerda se a chave for menor
 * E descer à direita se for maior
 * Chamar a inserção recursiva ro filho escolhido
 * Se a inserção recursiva retornar uma chave promovida, precisa tentar inserir essa chave promovida no nó atual
 * Retornar chave promovida ou um valor NULL se não houver promoção
*/
promotedKey *_bTreeInsert(bTreePage *node, promotedKey *key, FILE *bTreeFilePointer);


/* Função mais abstrata de inserção
 * Prepara os dados da nova chave
 * Tenta inserir recursivamente
 * Se tiver chave promovida no final da recursão, é que existe nova raíz
 * Chama as funções para criar nova raíz e atualizar o cabeçalho
*/
Errors bTreeInsert(PrimaryIndex *newRecord, bTreePage *root, FILE *bTreeFilePointer);
// Função antiga:Errors bTreeInsert(record *record, bTreePage *root, FILE *bTreeFile);

/* Procura no nó atual se a chave existe
 * Se não existir, tenta procurar no filho adequado, recursivamente
 * Se encontrar a chave, retorna RRN dela
 * Se não encontrar (chegar num nó folha e não estiver lá), retorna -1
*/
long bTreeSearch(bTreePage *node, int key, FILE* bTreeFilePointer);


//Funções antigas:
/* Coleta o nó raiz
 * Chama a função de busca
 * Caso seja encontrado, re-escreve as informações no nó e salva o arquivo
 * Caso não seja encontrado retorna erro
*/ 
//Errors bTreeUpdate(record *record, bTreePage *root, FILE *bTreeFile);

/* Coleta o nó raíz
 * Busca pela chave na página
 * Caso não encontre, busca na subárvore apropriada
 * Recursão até encontrar a chave 
*/
//long bTreeSearch(bTreePage *root, int key, FILE *bTreeFile);


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





#endif
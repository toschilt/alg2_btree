#ifndef __BTREE_H__
#define __BTREE_H__

#include <stdio.h>
#include <stdlib.h>

//TAMANHO DA PÁGINA: 4096 Bytes
#define PAGESIZE 4096

typedef struct {
    //Estrutura para armazenar possíveis erros (Ideia tirada do PDF do trabalho)
} Errors;

typedef struct {
    int key, RRN;
} record;

typedef struct {
    record *records;
    long *childs;
    int keys;
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
long bTreeSelect(bTreePage *node, int key, FILE* bTreeFilePointer);


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

#endif
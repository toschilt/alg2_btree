#include "btree.h"
#include "streamHandler.h"
#include <string.h>

#define BTREEFILENAME "btree.dat"

#define EXITFUNCTIONNAME "exit"
#define SEARCHFUNCTIONNAME "search"
#define INSERTFUNCTIONNAME "insert"
#define UPDATEFUNCTIONNAME "update"


void readString(char *string, char separator) {
    char buf;
    int i = 0;
    do {
        buf = fgetc(stdin); //Lê a próxima entrada
        string[i] = buf; //Adiciona à string
        i++;
    } while(buf != '\n' && buf != EOF && buf != '\0' && buf != separator);
    //Enquanto não for lido algum dos acima, a leitura da string continua
    i--;
    string[i] = '\0'; //Adiciona o \0 no local devido e retorna
}



int main(int argc, char *argv[]) {
    
    char function[20];

    while(1) { //Loop infinito
        readString(function, ' '); //Lê a entrada do usuário, passando o espaço como separador
        
        if(!strcmp(function, EXITFUNCTIONNAME)) { //Encerrar o programa (exit)
            //DESALOCA TUDO
            //ENCERRA
            printf("Encerra\n");
            return; //Encerra o programa
        }

        else if(!strcmp(function, INSERTFUNCTIONNAME)) { //Inserir novo estudante (insert)
            //CUIDA DA PARTE DE INSERÇÃO
            printf("insere\n");
        }

        else if(!strcmp(function, SEARCHFUNCTIONNAME)) {
            //CUIDA DA BUSCA
            printf("buscar\n");
        }

        else if(!strcmp(function, UPDATEFUNCTIONNAME)) {
            //CUIDA DA ATUALIZAÇÃO
            printf("Atualizar\n");
        }
    }
}
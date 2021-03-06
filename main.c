#include "streamHandler.h"
#include "publicInterface.h"
#include "student.h"

#define EXITFUNCTIONNAME "exit"
#define SEARCHFUNCTIONNAME "search"
#define INSERTFUNCTIONNAME "insert"
#define UPDATEFUNCTIONNAME "update"

int main(int argc, char *argv[]) {
    char function[20];

    //Criação dos documentos binários, pode ser melhorado
    FILE *fp = fopen(BTREEFILENAME, "w");
    fclose(fp);

    FILE *dataf = fopen(DATAFILENAME, "w");
    fclose(dataf);

    while(1) { //Loop infinito
        readString(function, ' '); //Lê a entrada do usuário, passando o espaço como separador
        
        if(!strcmp(function, EXITFUNCTIONNAME)) { //Encerrar o programa (exit)
            return 0; //Encerra o programa
        }

        else if(!strcmp(function, INSERTFUNCTIONNAME)) { //Inserir novo estudante (insert)
            studentRegister *student = (studentRegister*)malloc(getStudentSize());
            student = readStudentFromUser();
            insert(student);
        }

        else if(!strcmp(function, SEARCHFUNCTIONNAME)) { //Buscar por estudante (search)
            int key;
            scanf("%d", &key);
            search(key);
        }

        else if(!strcmp(function, UPDATEFUNCTIONNAME)) { //Atualizar informações de estudante (update)
            studentRegister *student = (studentRegister*)malloc(getStudentSize());
            student = readStudentFromUser();
            update(student);
        }
    }
}
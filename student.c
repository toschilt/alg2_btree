#include "streamHandler.h"
#include "bTreeUtils.h"
#include "student.h"


int getStudentSize() {
    //Retorna o tamanho da esturtura studentRegister
    return (sizeof(int) + sizeof(float) + (NOMESIZE + SOBRENOMESIZE + CURSOSIZE) * sizeof(char));
}


void printStudent(studentRegister *student) {
    //Imprime as informações no formato fornecido
    printf("−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−\n");
    printf("nUSP: %d\n", student->nusp);
    printf("Nome: %s\n", student->nome);
    printf("Sobrenome: %s\n", student->sobrenome);
    printf("Curso: %s\n", student->curso);
    printf("Nota: %.2f\n", student->nota);
    printf("-------------------------------\n");
}
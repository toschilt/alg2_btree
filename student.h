#ifndef __STUDENT_H__
#define __STUDENT_H__

//Alterar os valores
#define NOMESIZE 10
#define SOBRENOMESIZE 10
#define CURSOSIZE 10

typedef struct {
    int nusp;
    char nome[NOMESIZE];
    char sobrenome[SOBRENOMESIZE];
    char curso[CURSOSIZE];
    float nota;
} studentRegister;

//int getStudentSize()
//printStudent(student)


#endif
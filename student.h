#ifndef __STUDENT_H__
#define __STUDENT_H__

/*
 * Algoritmos e Estruturas de Dados II
 * Autores: Lucas Toschi, Murilo Todão e Marcos Ordonha (Grupo 15)
 * 
 * STUDENT.H
 * Neste arquivo estão as funções e estruturas que definem e operam
 * sobre o que são consideradas informações de um estudante 
*/

//Define do tamanho dos campos para cada estudante
#define NOMESIZE 25
#define SOBRENOMESIZE 25
#define CURSOSIZE 25


/*
 * Struct responsável por armazenar os dados de cada estudante.
*/
typedef struct {
    int nusp;
    char nome[NOMESIZE];
    char sobrenome[SOBRENOMESIZE];
    char curso[CURSOSIZE];
    float nota;
} studentRegister;

/*
 * Retorna o tamanho da struct com as informações de cada estudante.
*/
int getStudentSize();

/*
 * Imprime na tela as informações do estudante de forma formatada.
*/
void printStudent(studentRegister *student);


#endif
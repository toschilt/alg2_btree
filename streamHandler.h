#ifndef __STREAMHANDLER_H__
#define __STREAMHANDLER_H__

#define NOMESIZE 10
#define SOBRENOMESIZE 10
#define CURSOSIZE 10
//Alterar os valores

typedef struct {
    int nusp;
    char nome[NOMESIZE];
    char sobrenome[SOBRENOMESIZE];
    char curso[CURSOSIZE];
    float nota;
} studentRegister;


//Função para salvar registro em arquivo de dados
//Função para coletar registro do arquivo de dados a partir do RRN

//Cuidar de strings de entrada (Talvez em arquivo separado?)



#endif
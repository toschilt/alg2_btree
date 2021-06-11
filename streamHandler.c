#include "streamHandler.h"

//ATUALIZAR
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


studentRegister *searchStudentInDataFile(long RRN) {
    FILE *filePointer = fopen(DATAFILENAME, "r");
    studentRegister *student = (studentRegister*)malloc(getStudentSize());
    fseek(filePointer, RRN * getStudentSize(), SEEK_SET);

    fread(&student->nusp, sizeof(student->nusp), 1, filePointer);
    fread(&student->nome, sizeof(student->nome), 1, filePointer);
    fread(&student->sobrenome, sizeof(student->sobrenome), 1, filePointer);
    fread(&student->curso, sizeof(student->curso), 1, filePointer);
    fread(&student->nota, sizeof(student->nota), 1, filePointer);
    fclose(filePointer);

    return student;
}


studentRegister *readStudentFromUser() {
    studentRegister *student = (studentRegister*)malloc(getStudentSize());
    char c;
    scanf("%d%c", &student->nusp, &c);
    readString(student->nome, ',');
    readString(student->sobrenome, ',');
    readString(student->curso, ',');
    scanf("%f", &student->nota);
    return student;
}


void insertStudentInDataFile(studentRegister *student) {
    FILE *filePointer = fopen(DATAFILENAME, "a");
    fseek(filePointer, 0, SEEK_END);
    fwrite(&student->nusp, sizeof(int), 1, filePointer);
    fwrite(student->nome, NOMESIZE * sizeof(char), 1, filePointer);
    fwrite(student->sobrenome, SOBRENOMESIZE * sizeof(char), 1, filePointer);
    fwrite(student->curso, CURSOSIZE * sizeof(char), 1, filePointer);
    fwrite(&student->nota, sizeof(float), 1, filePointer);
    fflush(filePointer);
    fclose(filePointer);
}


long getRecordsInDataFile() {
    FILE *filePointer = fopen(DATAFILENAME, "a");
    fseek(filePointer, 0, SEEK_END);
    long fileSize = ftell(filePointer);
    fclose(filePointer);
    return (fileSize / getStudentSize());
}



void insertNodeInBTreeFile(bTreePage *bPage, FILE *bFile, long RRN) {
    
    if(RRN == -1) { fseek(bFile, 0, SEEK_END); } //Caso RRN seja -1, insere no fim
    else { fseek(bFile, RRN * PAGESIZE, SEEK_SET); } //Caso contrário, insere em posição específica

    fwrite(bPage, PAGESIZE, 1, bFile);
    fflush(bFile);
}



bTreePage *getPageFromBTreeFile(long RRN) {
    FILE *filePointer = fopen(BTREEFILENAME, "r+");
    bTreePage *bPage = (bTreePage*)malloc(PAGESIZE);

    fseek(filePointer, RRN * PAGESIZE, SEEK_SET);    
    fread(bPage, PAGESIZE, 1, filePointer);

    fclose(filePointer);
    return bPage;
}
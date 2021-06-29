#include "streamHandler.h"


void readString(char *string, char separator) {
    char currentChar;
    int currentInputIndex = 0;

    do {
        currentChar = fgetc(stdin); //Lê a próxima entrada
        string[currentInputIndex] = currentChar; //Adiciona à string
        currentInputIndex++;
    } while(currentChar != '\n' && currentChar != EOF && currentChar != '\0' && currentChar != separator);
    //Enquanto não for lido algum dos acima, a leitura da string continua
    
    currentInputIndex--;
    string[currentInputIndex] = '\0'; //Adiciona o \0 no local devido e retorna
}



studentRegister *searchStudentInDataFile(long RRN) {
    FILE *filePointer = fopen(DATAFILENAME, "r");
    studentRegister *student = (studentRegister*)malloc(getStudentSize());
    fseek(filePointer, RRN * getStudentSize(), SEEK_SET);
    //Abre o arquivo na posição fornecida

    fread(&student->nusp, sizeof(student->nusp), 1, filePointer);
    fread(&student->nome, sizeof(student->nome), 1, filePointer);
    fread(&student->sobrenome, sizeof(student->sobrenome), 1, filePointer);
    fread(&student->curso, sizeof(student->curso), 1, filePointer);
    fread(&student->nota, sizeof(student->nota), 1, filePointer);
    fclose(filePointer);
    //Realiza a leitura do arquivo de dados

    return student;
}


void removeAllChars(char* string, char c) {
    char *readPointer = string, *writePointer = string;
    while(*readPointer) {
        *writePointer = *readPointer++;
        writePointer += (*writePointer != c);
    }
    *writePointer = '\0';
}


studentRegister *readStudentFromUser() {
    studentRegister *student = (studentRegister*)malloc(getStudentSize());
    char c;
    scanf("%d%c", &student->nusp, &c);

    char tempNome[NOMESIZE];
    char tempSobrenome[SOBRENOMESIZE];
    char tempCurso[CURSOSIZE];

    readString(tempNome, ',');
    readString(tempSobrenome, ',');
    readString(tempCurso, ',');

    removeAllChars(tempNome, '"');
    removeAllChars(tempSobrenome, '"');
    removeAllChars(tempCurso, '"');

    strcpy(student->nome, tempNome);
    strcpy(student->sobrenome, tempSobrenome);
    strcpy(student->curso, tempCurso);
    
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


void updateStudent(studentRegister *student, long RRN) {
    FILE *filePointer = fopen(DATAFILENAME, "r+");
    fseek(filePointer, RRN * getStudentSize(), SEEK_SET);
    fwrite(&student->nusp, sizeof(int), 1, filePointer);
    fwrite(student->nome, NOMESIZE * sizeof(char), 1, filePointer);
    fwrite(student->sobrenome, SOBRENOMESIZE * sizeof(char), 1, filePointer);
    fwrite(student->curso, CURSOSIZE * sizeof(char), 1, filePointer);
    fwrite(&student->nota, sizeof(float), 1, filePointer);
    fflush(filePointer);
    fclose(filePointer);
}


long getRecordsInDataFile() {
    FILE *filePointer = fopen(DATAFILENAME, "r+");
    fseek(filePointer, 0, SEEK_END);
    long fileSize = ftell(filePointer);
    fclose(filePointer);
    return (fileSize / getStudentSize());
}



void insertNodeInBTreeFile(bPageInfo *newPage, FILE *bFile, long RRN) {
    if(RRN == -1) { fseek(bFile, 0, SEEK_END); } //Caso RRN seja -1, insere no fim
    else { fseek(bFile, RRN * PAGESIZE, SEEK_SET); } //Caso contrário, insere em posição específica
    
    fwrite(newPage, PAGESIZE, 1, bFile);
    fflush(bFile);
}



bPageInfo *getPageFromBTreeFile(long RRN) {
    FILE *filePointer = fopen(BTREEFILENAME, "r+");
    bPageInfo *newPage = (bPageInfo*)malloc(PAGESIZE);

    fseek(filePointer, RRN * PAGESIZE, SEEK_SET);    
    fread(newPage, PAGESIZE, 1, filePointer);

    fclose(filePointer);
    return newPage;
}
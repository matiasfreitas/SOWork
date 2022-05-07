//
// Created by Matias Freitas Guimarães on 26/04/2022.
//

#include "addmx.h"

void addline(int column,int nLines, int nColumns, int* matrix1, int* matrix2, int* matrixResult){
    for (int i = 0; i < nLines; ++i) {
        matrixResult[nColumns*i + column] = matrix1[nColumns*i + column] + matrix2[nColumns*i + column];
    }
    return;
}


int* addMatrix(int nLines, int nColumns, int* matrix1, int* matrix2, int offsetMatrix1, int offsetMatrix2){
    int *matrixResult = mmap(NULL,nColumns*nLines*sizeof(int),
                             PROT_WRITE|PROT_READ,
                             MAP_SHARED | MAP_ANONYMOUS,
                             0,0);

    if(matrixResult == MAP_FAILED){
        printf("Result Matrix Failed\n");
        exit(1);
    }

    pid_t pids[nColumns];
    int n = nColumns;
    for (int j = 0; j < nColumns; ++j) {
        pids[j] = fork();
        if (pids[j] < 0) {
            perror("fork");
            abort();
        } else if (pids[j] == 0) {
            addline(j, nLines, nColumns, matrix1, matrix2, matrixResult);
            exit(0);
        }
    }
    int status;
    while (n > 0) {
        wait(&status);
        --n;
    }
    return matrixResult;
}

int readNumber(char* matrix, int *offset, char sep){
    int number = matrix[*offset] - 48;
    int nDigitsMatrix = *offset+1;
    while(matrix[nDigitsMatrix] != sep){
        number = number * 10 + matrix[nDigitsMatrix] - 48;
        nDigitsMatrix++;
    }
    nDigitsMatrix++;
    *offset = nDigitsMatrix;
    return number;
}

int* readMatrix(int nLines, int nColumns,int* offsetMatrix, char* matrixContent){
    int *matrixResult = mmap(NULL,nColumns*nLines*sizeof(int),
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS,
                             0,0);

    if(matrixResult == MAP_FAILED){
        printf("Read Matrix Failed\n");
        exit(1);
    }

    for (int i = 0; i < nLines-1; ++i) {
        for (int j = 0; j < nColumns-1; ++j) {
            matrixResult[nColumns*i + j] = readNumber(matrixContent, offsetMatrix, ' ');
        }
        matrixResult[nColumns*(i+1)-1] = readNumber(matrixContent, offsetMatrix, '\n');
    }
    for (int j = 0; j < nColumns-1; ++j) {
        matrixResult[(nLines-1)*nColumns + j] = readNumber(matrixContent, offsetMatrix, ' ');
    }
    matrixResult[nLines*nColumns-1] = readNumber(matrixContent, offsetMatrix, '\0');
    return matrixResult;
}

int main(int argc, char *argv[]){
    if(argc == 1){
        printf("usage: addmx file1 file2\n");
        exit(0);
    }
    else if(argc < 3){
        printf("File path not mentioned\n");
        exit(0);
    }
    const char *filepathMatrix1 = argv[1];
    int fileMatrix1 = open(filepathMatrix1, O_RDWR);
    if(fileMatrix1 < 0){
        printf("\n\"%s \" could not open\n",
               filepathMatrix1);
        exit(1);
    }
    const char *filepathMatrix2 = argv[2];
    int fileMatrix2 = open(filepathMatrix2, O_RDWR);
    if(fileMatrix2 < 0){
        printf("\n\"%s \" could not open\n",
               filepathMatrix2);
        exit(1);
    }
    struct stat statbufMatrix1;
    int errMatrix1 = fstat(fileMatrix1, &statbufMatrix1);
    if(errMatrix1 < 0){
        printf("\n\"%s \" could not open\n",
               filepathMatrix1);
        exit(2);
    }
    struct stat statbufMatrix2;
    int errMatrix2 = fstat(fileMatrix1, &statbufMatrix2);
    if(errMatrix2 < 0){
        printf("\n\"%s \" could not open\n",
               filepathMatrix2);
        exit(2);
    }

    char *Matrix1Content = mmap(NULL,statbufMatrix1.st_size,
                                PROT_READ,
                                MAP_SHARED,
                                fileMatrix1,0);
    if(Matrix1Content == MAP_FAILED){
        printf("Mapping Matrix 1 Content Failed\n");
        exit(1);
    }

    char *Matrix2Content = mmap(NULL,statbufMatrix2.st_size,
                                PROT_READ,
                                MAP_SHARED,
                                fileMatrix2,0);
    if(Matrix2Content == MAP_FAILED){
        printf("Mapping Matrix 2 Content Failed\n");
        exit(1);
    }

    int matrix1Columns, matrix1Rows;
    int offsetMatrix1 = 0;
    matrix1Rows = readNumber(Matrix1Content, &offsetMatrix1, 'x');
    matrix1Columns = readNumber(Matrix1Content, &offsetMatrix1, '\n');
    int *Matrix1 = readMatrix(matrix1Rows, matrix1Columns, &offsetMatrix1, Matrix1Content);

    int matrix2Columns, matrix2Rows;
    int offsetMatrix2 = 0;
    matrix2Rows = readNumber(Matrix2Content, &offsetMatrix2, 'x');
    matrix2Columns = readNumber(Matrix2Content, &offsetMatrix2, '\n');
    int *Matrix2 = readMatrix(matrix2Rows, matrix2Columns, &offsetMatrix2, Matrix2Content);
    if(matrix1Rows == matrix2Rows && matrix1Columns == matrix2Columns){
        int* matrixResult = addMatrix(matrix1Rows, matrix1Columns, Matrix1, Matrix2, offsetMatrix1, offsetMatrix2);
        for (int i = 0; i < matrix1Rows; ++i) {
            for (int j = 0; j < matrix1Columns; ++j) {
                printf("%d ",matrixResult[matrix1Columns*i+ j]);
            }
            printf("\n");
        }
    }
    return 0;
}

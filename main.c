#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
//Matrix Dim
#define ROWS 4
#define COLS 8

int main(int argc, char **argv) {

    FILE *matrix;
    char *matrixPath = NULL;
    char *matrixFileContent = NULL;

    int **matrixArray;

    int matrixID[4][4] = {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
    };

    //Display the awesome program intro
    progIntro();

    do{
        // Ask Path, Load Matrix, Put in string, verify and format.
        do{
            //Ask for matrix file path and load the matrix.
            do{
                matrixPath = askMatrixPath();
                matrix = loadMatrixFile(matrixPath);
            }while(matrix == NULL);

            //Read the matrix file content and put in string.
            matrixFileContent = fileContentToString(matrix);
        }while(verifyContent(matrixFileContent));

        matrixArray = matrixAlloc(ROWS, COLS);
        setMatrix(matrixArray, matrixFileContent, ROWS, COLS);

        //Display the matrix
        puts("The matrix is : \n");
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 8; j++){
                printf("%d ", matrixArray[i][j]);
            }
            printf("\n");
        }
    }while(verifyMatrix(matrixArray,matrixID, ROWS, COLS));

    cypherByMatrix(matrixArray, ROWS, COLS/*, char *fileToCypher*/);

    free(matrixFileContent);
    free(matrixArray);

    //Syncing test for github

    // End of Program
    system("pause");
    return 0;

}
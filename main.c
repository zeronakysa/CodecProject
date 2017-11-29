#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
//Matrix Dim
#define ROWS 4
#define COLS 8

int main(int argc, char **argv) {

    // Matrix Related Variables
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
    // File related var
    char *filePath = NULL;
    //Other variables
    int choice = 0;

    //Display the awesome program intro
    progIntro();

    //---------------------------------------------------------------------------------------------
    //------------------- Ask for the matrix File before choosing what to do ----------------------
    //---------------------------------------------------------------------------------------------
    do{
        do{
            //Ask for matrix file path and load the matrix.
            do{
                matrixPath = askMatrixPath();
                matrix = loadMatrixFile(matrixPath);
            }while(matrix == NULL);
            //Read the matrix file content and put in string.
            matrixFileContent = fileContentToString(matrix);
        }while(verifyContent(matrixFileContent));
        //Allocate memory for 2D Array
        matrixArray = matrixAlloc(ROWS, COLS);
        //Pour the file content in the matrix
        setMatrix(matrixArray, matrixFileContent, ROWS, COLS);

        // Display
//        for(int i = 0; i < 4; i++){
//            for(int j = 0; j < 8; j++){
//                printf("%d ", matrixArray[i][j]);
//            }
//            printf("\n");
//        }
    }while(verifyMatrix(matrixArray,matrixID, ROWS, COLS));

    //---------------------------------------------------------------------------------------------
    //-------------------------------- END OF MATRIX INSTRUCTIONS ---------------------------------
    //---------------------------------------------------------------------------------------------

    choice = menu();

    switch(choice){
        case 1:
            filePath = askFilePath();
            cypherByMatrix(matrixArray, ROWS, COLS, filePath);
            break;
        case 2:
            filePath = askFilePath();
            decipher(matrixArray, matrixID, ROWS, COLS, filePath);
            break;
        case 3:
            puts("Bye bye !");
            break;
        default:
            break;

    }

    free(matrixArray);
    free(filePath);

    // End of Program
    system("pause");
    return 0;
}
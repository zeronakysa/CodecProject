//
// Created by Zero on 22/11/2017.
//

#ifndef CODECPROJECT_FUNCTIONS_H
#define CODECPROJECT_FUNCTIONS_H



//Display the awesome program intro
void progIntro();

//Clear the buffer
void clearBuffer();

//Ask for the path of matrix
char *askMatrixPath();

//Open the matrix file
FILE *loadMatrixFile(char *);

//Return content of file in a string
char *fileContentToString(FILE *);

//Verify and Format the content of the matrix file.
int verifyContent(char *);

//Create the matrix and initialize it
int **matrixAlloc(int, int);

//Set the matrix
void setMatrix(int *[], char*,int, int);

//Verify the content of matrix, return 1 if error
int verifyMatrix(int *[], int[][4], int, int);

int cypherByMatrix(int *matrix[], int rows, int cols/*, char *fileToCypher*/);

#endif //INC_2017_ESGI_PROJECT_CIPHER_FUNCTIONS_H


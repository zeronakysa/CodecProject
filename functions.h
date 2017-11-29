#ifndef INC_2017_ESGI_PROJECT_CIPHER_FUNCTIONS_H
#define INC_2017_ESGI_PROJECT_CIPHER_FUNCTIONS_H

int menu();

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

//Pour file content in the matrix
void setMatrix(int *[], char*,int, int);

//Verify the content of matrix, return 1 if error
int verifyMatrix(int *[], int[][4], int, int);

//Ask for the file path (cipher or decipher)
char *askFilePath();

int cypherByMatrix(int *[], int, int, char *);

int *colsMatching(int *[], int[][4], int, int);

int decipher(int *[], int[][4], int, int, char *);

#endif //INC_2017_ESGI_PROJECT_CIPHER_FUNCTIONS_H

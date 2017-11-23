#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

void progIntro(){
    puts("+------------------------------------+");
    puts("|           Codec Project            |");
    puts("|             By Group4              |");
    puts("|         Aurelien Delagarde         |");
    puts("|         Steven   Cantagrel         |");
    puts("+------------------------------------+");

    puts(" ");
    puts("--------------------------------------------------------------------------------------------");
    puts("           The purpose of this program is to cipher or decipher any file you want.          ");
    puts("All you need to do is put the files in the same folder as the .exe and you are good to go ;)");
    puts("--------------------------------------------------------------------------------------------");
    puts(" ");
    puts("+--------------------------------------------------------------------------------------------------+");
    puts("| 1.                                  ENTER THE MATRIX                                             |");
    puts("|                                                                                                  |");
    puts("|The program will ask for a key.txt file containing the G4 matrix. The matrix can be hashed or not.|");
    puts("|The matrix should look like that G4C=[XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX]                        |");
    puts("+--------------------------------------------------------------------------------------------------+");
    puts(" ");
    puts("+-------------------------------------------------------------+");
    puts("| 2.               ENTER THE FILE TO CIPHER                   |");
    puts("|The file can be of any type and size(.txt,.mp3,.mp4,.bin,...)|");
    puts("+-------------------------------------------------------------+");
    puts(" ");
    puts("+-----------------------------------------------------------------------------------------+");
    puts("| 3.                              GET YOUR CIPHERED FILE                                  |");
    puts("|Your freshly ciphered file will wait for you in the .exe folder next to your source file |");
    puts("|It should have a *.*c extension.                                                         |");
    puts("+-----------------------------------------------------------------------------------------+");
    puts(" ");
    puts("+-------+");
    puts("| ENJOY |");
    puts("+-------+");
    puts(" ");
    puts("Deciphering follows the same step as ciphering.");
    puts("You only need to load an already ciphered *.*c extension file and you'll get a *.*cd file in result.");
}

void clearBuffer(){
    int c = 0;
    while (c != '\n' && c != EOF)
        c = getchar();
}

char * askMatrixPath(){

    char pathTmp[255];
    char *matrixPath = NULL;
    puts("Enter the matrix filename (with extension).");
    fgets(pathTmp, 255, stdin);

    if(strlen(pathTmp) < 255)
        pathTmp[strlen(pathTmp) - 1] = '\0';

    matrixPath = malloc(strlen(pathTmp) + 1);

    if(matrixPath == NULL){
        puts("Matrix path memory alloc error.");
        exit(EXIT_FAILURE);
    }

    strcpy(matrixPath, pathTmp);
    return matrixPath;
}

FILE *loadMatrixFile(char *matrixPath){

    FILE *matrix;
    matrix = fopen(matrixPath, "rb");
    if(matrix == NULL){
        puts("Can't open file. File not found.");
        return NULL;
    }

    puts("Matrix file successfully loaded.");
    free(matrixPath);
    return matrix;
}

char *fileContentToString(FILE *matrix){

    long length;
    char *buffer = NULL;

    fseek(matrix, 0, SEEK_END);
    length = ftell(matrix);
    fseek(matrix, 0, SEEK_SET);
    buffer = malloc((length +1) * sizeof(char));

    if(buffer == NULL){
        puts("Buffer memory allocation error.");
        exit(EXIT_FAILURE);
    }else{
        fread(buffer, sizeof(char), length, matrix);
    }

    fclose(matrix);
    buffer[length] = 0;

    return buffer;
}

int verifyContent(char *matrixFileContent){

    int error = 0;
    char *p = matrixFileContent;

    //Verify G4C=[ is the start of the content
    if(strncmp(matrixFileContent, "G4C=[", 5) != 0){
        puts("Matrix file content should start with G4C=[");
        error = 1;
    }

    //Verify that ] is at the end
    if(matrixFileContent[strlen(matrixFileContent) - 1] != ']'){
        puts("Matrix file content should end with ]");
        error = 1;
    }

    //Delete G4C=[ and ]
    while( (p = strpbrk(p, "G4C=[]")) != NULL){
        strcpy(p, p+1);
    }

    //Delete spaces between bytes
    while( (p = strchr(matrixFileContent, ' ')) != NULL){
        strcpy(p, p+1);
    }

    //Verify number of bit = 32
    if(strlen(matrixFileContent) != 32){
        puts("You don't have 32 bits inside the brackets []");
        error = 1;
    }

    if(error)
        puts("Please edit your matrix file.");

    return error;
}

int **matrixAlloc(int rows, int cols){

    int i, j = 0;
    int **matrixArray;

    matrixArray = malloc(rows * sizeof(int));
    for(i = 0; i < rows; i++){
        matrixArray[i] = malloc(cols * sizeof(int));
    }

    if(matrixArray == NULL){
        puts("Matrix Array Allocation Error.");
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            matrixArray[i][j] = 0;
        }
    }

    return matrixArray;
}

void setMatrix(int *matrix[], char *matrixFileContent,int rows, int cols){

    int i, j, k = 0;
    char oneChar[2];
    int val = 0;


    for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            oneChar[0] = matrixFileContent[k];
            val = atoi(oneChar);
            matrix[i][j] = val;
            k++;
        }
    }
}

int verifyMatrix(int *matrix[], int matrixID[][4], int rows, int cols){

    int i, j, k = 0; // i = rows ; j = cols ; k = index when comparing two cols
    int error = 0;
    int count = 0;
    int colsCount = 0; // Used to identify matrixID in matrix

    //Verify if rows != 0
    for(i = 0; i < rows; i++){
        count = 0;
        for(j = 0; j < cols; j++){
            if(matrix[i][j] == 0)
                count++;

        }
        if(count == cols){
            puts("One row is only 0.");
            error = 1;
            break;
        }
    }

    //Verify if cols != 0
    for(j = 0; j < cols; j++){
        count = 0;
        for(i = 0; i < rows; i++){
            if(matrix[i][j] == 0)
                count++;
        }
        if(count == rows){
            puts("One column is only 0");
            error = 1;
            break;
        }
    }

    //Verify equality of columns (version 2)
    for(j = 0; j < cols; j++){
        for(k = j + 1; k < cols; k++){
            count = 0;
            for(i = 0; i < rows; i++){
                if(matrix[i][j] == matrix[i][k])
                    count++;

            }
            if(count == rows){
                puts("Two columns or more are equal.");
                j = cols + 1;
                error = 1;
                break;
            }
        }
    }

    //Verify matrixID is in matrix

    //Loop on cols of matrixID
    for(j = 0; j < 4; j++){
        //Loop on cols of matrix
        for(k = 0; k < cols; k++){
            count = 0;
            //Loop on rows for both matrix
            for(i = 0; i < rows; i++){
                // Count if one cols of matrix is equal to matrixID
                if(matrixID[i][j] == matrix[i][k])
                    count++;
            }
            // If cols equal : colsCount++
            if(count == rows){
                colsCount++;
                break;
            }
        }
    }
    // If not all matrixID cols found : error = 1
    if(colsCount != 4){
        puts("Identity Matrix not found in matrix.");
        error = 1;
    }

    if(error)
        puts("Please edit your matrix file.");

    return error;
}



int cypherByMatrix(int *matrix[], int rows, int cols/*, char *fileToCypher*/){

    unsigned char byte; // Byte content
    unsigned char byteCypher; // Byte content cyphered
    unsigned char bits[8]; // Byte content in array
    unsigned char mask = 1; // Bit mask
    int cipherBinary[8]; // Binary cipher with matrix
    int i, j, k = 0; // indice

    FILE *fileInput;
    FILE *fileOutput;

    fileInput = fopen("mp4.mp4","rb");

    char *fileToCypher = "mp4.mp4";
    char *cypherFile = "mp4.mp4c"; //Tempory -> strcat f

    fileOutput = fopen(cypherFile, "wb+");

    while(fread(&byte, sizeof(unsigned char), 1, fileInput), !feof(fileInput)) {

        k = 0; // resest of k
        byteCypher = 0; //reset the byte cyphered for the next one
        // reset of cipherBinary to 00000000 for the case of 0
        for (j = 0; j < 8; j++) {
            cipherBinary[j] = 0;
        }
        // transform byte(8bit) in array Part 1/2
        for (int i = 0; i < rows; i++) {
            bits[i] = byte & (mask << i); // extract ^2 from byte and stock to bits[i] We can get 0 or 1 / 2 / 4 / 8 / 16 / 32 / 64 / 128
            if(bits[i] != 0) // if we got 1 / 2 / 4 / 8 / 16 / 32 / 64 / 128
                bits[i] = 1; // we transform it in 1 for a good result. ( Good result is 0101. Bad result is 0208 )
            if (bits[i]) {
                // If we get a number, we set the same rule as we did on paper , "Line On/Off", if the number is 1, the line is "On" and we add it to the final result if is 0, the line is "Off" so we ignore the line.
                for (j = 0; j < 8; j++) {
                    cipherBinary[j] = cipherBinary[j] + matrix[k][j];
                }
            }
            // k is the counter (max 4) for the choice of the good line into the matrix
            k++;
        }
        for (j = 0; j < 8; j++) {
            cipherBinary[j] = cipherBinary[j] % 2; // if we got 0103 0301 we transform it in 0101 0101 (1 + 1 + 1 = 11 so we catch the 1)
            byteCypher += cipherBinary[j]; // add the cyphered value to the final byte
//            printf("%d",cipherBinary[j]); // temporary
        }
        fwrite(&byteCypher,sizeof(unsigned char), 1,fileOutput);

        for (j = 0; j < 8; j++) {
            cipherBinary[j] = 0;
        }
        k = 0;
        byteCypher = 0; //reset the byte cyphered for the next one
        // transform byte(8bit) in array Part 2/2
        for (int i = 4; i < 8; i++) {
            bits[i] = byte & (mask << i); // extract ^2 from byte and stock to bits[i] We can get 0 or 1 / 2 / 4 / 8 / 16 / 32 / 64 / 128
            if(bits[i] != 0) // if we got 1 / 2 / 4 / 8 / 16 / 32 / 64 / 128
                bits[i] = 1; // we transform it in 1 for a good result. ( Good result is 0101. Bad result is 0208 )
            if (bits[i]) {
                // If we get a number, we set the same rule as we did on paper , "Line On/Off", if the number is 1, the line is "On" and we add it to the final result if is 0, the line is "Off" so we ignore the line.
                for (j = 0; j < 8; j++) {
                    cipherBinary[j] = cipherBinary[j] + matrix[k][j];
                }
            }
            // k is the counter (max 4) for the choice of the good line into the matrix
            k++;
        }
        for (j = 0; j < 8; j++) {
            cipherBinary[j] = cipherBinary[j] % 2; // if we got 0103 0301 we transform it in 0101 0101 (1 + 1 + 1 = 11 so we catch the 1)
            byteCypher += cipherBinary[j]; // add the cyphered value to the final byte
//            printf("%d",cipherBinary[j]); // temporary
        }
        fwrite(&byteCypher,sizeof(unsigned char), 1,fileOutput);
    }
    fclose(fileInput); // close the input file

    fclose(fileOutput); //close the output file

    return 0;
}
/*

int main2(int argc, char **argv) {

    int matrix[4][8];
    matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = 0; matrix[0][3] = 0; matrix[0][4] = 1; matrix[0][5] = 1; matrix[0][6] = 1; matrix[0][7] = 0;
    matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = 0; matrix[1][3] = 0; matrix[1][4] = 1; matrix[1][5] = 1; matrix[1][6] = 0; matrix[1][7] = 1;
    matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1; matrix[2][3] = 0; matrix[2][4] = 1; matrix[2][5] = 0; matrix[2][6] = 1; matrix[2][7] = 1;
    matrix[3][0] = 0; matrix[3][1] = 0; matrix[3][2] = 0; matrix[3][3] = 1; matrix[3][4] = 0; matrix[3][5] = 1; matrix[3][6] = 1; matrix[3][7] = 1;

}
 */

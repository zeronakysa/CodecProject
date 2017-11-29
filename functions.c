#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

int menu(){
    int choice = 0;

    puts(" ");
    puts("Selection Menu");
    puts("1. Cipher a file");
    puts("2. Decipher a file");
    puts("3. Exit program.");
    puts(" ");
    puts("Enter a choice :");

    if(scanf("%d", &choice) != 1){
        perror("Error ");
        exit(EXIT_FAILURE);
    }

    return choice;
}

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
    // Free the char* containing the matrix file content
    free(matrixFileContent);
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

char *askFilePath(){

    char pathTmp[255];
    char *filePath = NULL;

    puts("Enter the filename to cipher/decipher");
    clearBuffer();
    fgets(pathTmp, 255, stdin);

    if(strlen(pathTmp) < 255)
        pathTmp[strlen(pathTmp) - 1] = '\0';

    filePath = malloc((strlen(pathTmp) + 1));

    if(filePath == NULL){
        puts("File Path Allocation Error.");
        exit(EXIT_FAILURE);
    }

    strcpy(filePath, pathTmp);
    return filePath;
}

int cypherByMatrix(int *matrix[], int rows, int cols, char *filePath){

    unsigned char byte; // Byte content
    unsigned char byteCypher; // Byte content cyphered
    unsigned char bits[8]; // Byte content in array
    unsigned char mask = 1; // Bit mask
    int cipherBinary[8]; // Binary cipher with matrix
    int j, k = 0; // index


    FILE *fileInput;
    FILE *fileOutput;

    fileInput = fopen(filePath,"rb");

    //Filename *.*c
    char cipheredFileName[255] = {'\0'};
    strcpy(cipheredFileName, strcat(filePath,"c"));


    fileOutput = fopen(cipheredFileName, "wb+");

    if(fileInput == NULL || fileOutput == NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
    }

    puts("\nCipher in progress..\n");

    while(fread(&byte, sizeof(unsigned char), 1, fileInput), !feof(fileInput)) {

        k = 0; // resest of k
        byteCypher = 0; //reset the byte cyphered for the next one
        // reset of cipherBinary to 00000000 for the case of 0
        for (j = 0; j < 8; j++) {
            cipherBinary[j] = 0;
        }
        // transform byte(8bit) in array Part 1/2
        for (int i = 0; i < 4; i++) {
            bits[i] = byte & (mask << 7-i); // extract ^2 from byte and stock to bits[i] We can get 0 or 1 / 2 / 4 / 8 / 16 / 32 / 64 / 128
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
        j = 7;
        for (int i = 0; i < 8; i++) {
            cipherBinary[i] = cipherBinary[i] % 2; // if we got 0103 0301 we transform it in 0101 0101 (1 + 1 + 1 = 11 so we catch the 1)
            if (cipherBinary[i] == 0){  // add the cyphered value to the final byte
                byteCypher += 0;
            }else{
                byteCypher +=  mask << j;
            }
            j--;
        }
        fwrite(&byteCypher,sizeof(unsigned char), 1,fileOutput);

        for (j = 0; j < 8; j++) {
            cipherBinary[j] = 0;
        }
        k = 0;
        byteCypher = 0; //reset the byte cyphered for the next one
        // transform byte(8bit) in array Part 2/2
        for (int i = 4; i < 8; i++) {
            bits[i] = byte & (mask << 7-i); // extract ^2 from byte and stock to bits[i] We can get 0 or 1 / 2 / 4 / 8 / 16 / 32 / 64 / 128
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
        j = 7;
        for (int i = 0; i < 8; i++) {
            cipherBinary[i] = cipherBinary[i] % 2; // if we got 0103 0301 we transform it in 0101 0101 (1 + 1 + 1 = 11 so we catch the 1)
            if (cipherBinary[i] == 0){  // add the cyphered value to the final byte
                byteCypher += 0;
            }else{
                byteCypher +=  mask << j;
            }
            j--;
        }
        fwrite(&byteCypher,sizeof(unsigned char), 1,fileOutput);
    }
    puts("\nCipher complete.\n");
    fclose(fileInput); // close the input file
    fclose(fileOutput); //close the output file

    return 0;
}

int *colsMatching(int *matrix[], int matrixID[][4], int rows, int cols){
    int *colsMatch = NULL;
    int i, j, k, l = 0;
    int count = 0;

    colsMatch = malloc(4 * sizeof(int));

    if(colsMatch == NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
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
                colsMatch[l] = k;
                l++;
                break;
            }
        }
    }
    return colsMatch;
}

int decipher(int *matrix[], int matrixID[][4], int rows, int cols, char *filePath){

    int i, j ,k = 0;
    // Byte reading file
    unsigned char byte;
    // array for converted Byte var
    unsigned char bits[8] = {0};
    // Array containing deciphered byte
    unsigned char decipherBits[4] = {0};
    // Array containing two parts of decipherBits
    unsigned char decipherByte[8] = {0};
    // Result containing decipherBits binary into decimal
    unsigned char decipherByteInDec = 0;
    // Temporary var for array index
    int tmp = 0;
    // FileOutput path
    char decipheredFileName[255] = {'\0'};
    // Array containing cols matching MatrixID;
    int *colsArray = NULL;

    unsigned char mask = 1;
    // FILE var
    FILE *fileInput;
    FILE *fileOutput;

    // OPEN CIPHERED FILE FOR READING
    fileInput = fopen(filePath, "rb");

    if(fileInput == NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
    }

    // OPEN DECIPHERED FILE FOR WRITING
    strcpy(decipheredFileName, strcat(filePath, "d"));

    fileOutput = fopen(decipheredFileName, "wb+");

    if(fileOutput == NULL){
        perror("Error ");
        exit(EXIT_FAILURE);
    }

    // Gets cols from matrix matching matrixID
    colsArray = colsMatching(matrix, matrixID, rows, cols);

    // FIRST PART
    puts("\nDecipher in progress..\n");
    while(fread(&byte, sizeof(unsigned char), 1, fileInput), !feof(fileInput)){
        for(i = 7; i >= 0; i--){
            bits[i] =  byte % 2;
            byte /= 2;
        }

        for(i = 0; i < 4; i++){
            tmp = colsArray[i];
            decipherBits[i] = bits[tmp];
            decipherByte[i] = decipherBits[i];
        }

        // Read Second Byte
        fread(&byte, sizeof(unsigned char), 1, fileInput);

        for(i = 7; i >= 0; i--){
            bits[i] = byte % 2;
            byte /= 2;
        }

        for(i = 0; i < rows; i++){
            tmp = colsArray[i];
            decipherBits[i] = bits[tmp];
        }
        j = 0;
        for(i = 4; i < cols; i++){
            decipherByte[i] = decipherBits[j];
            j++;
        }

        k = 7;
        decipherByteInDec = 0;
        for (j = 0; j < cols; j++) {
            if (decipherByte[j] == 0){  // add the cyphered value to the final byte
                decipherByteInDec += 0;
            }else{
//                decipherByteInDec += decipherByte[j] * ((int) pow(2, k));
                decipherByteInDec +=  mask << k;
            }
            k--;
        }
        fwrite(&decipherByteInDec,sizeof(unsigned char), 1,fileOutput);

    }
    puts("\nDecipher complete.\n");

    fclose(fileInput);
    fclose(fileOutput);
    free(colsArray);
}
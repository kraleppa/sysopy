#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include "table.h"

//connects two strings
char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

//compares two files and saves the result to tmp.txt
void compareTwoFiles(char *fileName1, char *fileName2){
    char *command = concat("diff ", concat(fileName1, concat(" ", concat(fileName2, " > tmp.txt"))));
    system(command);
}

//counts how many operations is in the tmp.txt file
int getNumberOfOperations(){
    FILE *filePoiner = fopen("tmp.txt", "r");
    if (filePoiner == NULL){
        exit(EXIT_FAILURE);
    }

    char buffer[255];
    int numberOfOperations = 0;

    while (fgets(buffer, 255, filePoiner)){
        if (isdigit(buffer[0])){
            numberOfOperations++;
        }  
    }
    return numberOfOperations;
}

//initilaize Block of Editing operations based on tmp.txt
char **initializeBlockOfEditingOperations(){
    FILE *filePoiner = fopen("tmp.txt", "r");
    char buffer[255];
    if (filePoiner == NULL){
        exit(EXIT_FAILURE);
    }
    int numberOfOperations = getNumberOfOperations();
    char **block = calloc(numberOfOperations, sizeof(char*));
    char *operation = "null";
    int i = 0;
    while (fgets(buffer, 255, filePoiner)){
        if (isdigit(buffer[0])){
            if (i != 0){
                block[i - 1] = operation;
            }
            operation = "";
            operation = concat(operation, buffer);

            i++;
            
        } else {
            operation = concat(operation, buffer);
        }
    }
    block[i - 1] = operation;

    return block;
}

// struct Table {
//     char ***mainTable;
//     int mainTableLength;
//     int *operationsBlockLength;
// };


struct Table initializeTable(){
    struct Table table;
    table.mainTableLength = 0;
    table.operationsBlockLength = NULL;
    return table;
}

int addOperationsToTable(struct Table *table){
    if  (table == NULL){
        printf("Null pointer exception\n");
        return -1;
    }
    if (table -> mainTableLength == 0){
        table -> mainTableLength = 1;
        table -> operationsBlockLength = calloc(1, sizeof(int));
        table -> operationsBlockLength[0] = getNumberOfOperations();

        table -> mainTable = calloc(1, sizeof(char**));
        table -> mainTable[0] = initializeBlockOfEditingOperations();
        return 0;
    }

    table -> mainTableLength++;
    int length = table -> mainTableLength;
    table -> operationsBlockLength = realloc(table -> operationsBlockLength, length * sizeof(int));
    table -> operationsBlockLength[length - 1] = getNumberOfOperations();

    table -> mainTable = realloc(table -> mainTable, length * sizeof(char**));
    table -> mainTable[length - 1] = initializeBlockOfEditingOperations();
    return length - 1;
}

int getInformationsAboutBlock(struct Table *table, int index){
    if  (table == NULL){
        printf("Null pointer exception\n");
        return -1;
    }
    if (index > table -> mainTableLength - 1){
        printf("index is bigger than length of table!");
        return -1;
    }
    return table -> operationsBlockLength[index];
}

void deleteBlock(struct Table *table, int index){
    if  (table == NULL){
        printf("Null pointer exception\n");
        return;
    }
    if (index > table -> mainTableLength - 1){
        printf("index is bigger than length of table!");
        return;
    }
    char **blockTable = table -> mainTable[index];
    int blockTableLength = table -> operationsBlockLength[index];
    table -> operationsBlockLength[index] = -1;
    for (int i = 0; i < blockTableLength; i++){
        free(blockTable[i]);
    }
    free(blockTable);
}

void deleteOperation(struct Table *table, int mainIndex, int blockIndex){
    if  (table == NULL){
        printf("Null pointer exception\n");
        return;
    }
    if (mainIndex > table -> mainTableLength - 1){
        printf("index is bigger than length of table!");
        return;
    }
    if (blockIndex > table -> operationsBlockLength[mainIndex] - 1){
        printf("index is bigger than length of table!");
        return;
    }
    char **blockTable = table -> mainTable[mainIndex];
    free(blockTable[blockIndex]);
}

void showAllTable(struct Table table){
    for (int i = 0; i < table.mainTableLength; i++){
        printf("Operation block %d: \n", i);
        for (int j = 0; j < table.operationsBlockLength[i]; j++){
            if (table.mainTable[i][j] != NULL && isdigit(table.mainTable[i][j][0])){
                printf("   Operation %d: \n      %s", j, table.mainTable[i][j]);
            }
        }
    }
}
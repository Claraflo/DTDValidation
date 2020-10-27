#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "functions.h"


int main(){

    //Defining variables and directory structs
    DIR *dir;
    struct dirent *entry;
    int i = 0;

    //Checking filesystem for valid XML files
    puts("\nLooking for XML files...");

    if((dir = opendir("xml")) == NULL){
        perror("Error opening the XML folder.");
    }
    else{
        while(((entry = readdir(dir))) != NULL){
            if(isXML(entry->d_name)){
                i++;                
                printf("| %s\n", entry->d_name);
            }
        }
        //printf("Found %d files. \n", i);
        closedir(dir);
    }

    //User inputs desired file
    FILE *file;
    char *fileName = (char*)malloc(34 * sizeof(char));
    strcpy(fileName, "xml/");
    char input[30];

    puts("\nPlease enter the name of the file:");
    while(1){
        scanf("%s", input);
        strcat(fileName, input);
        file = fopen(fileName, "r");

        if(file != NULL){
            break;
        }
        else{
            puts("File non valid.");
            memset(fileName+4,0,strlen(fileName)-4);
        }
    };
    
    // Counting lines and indexing

    int fileLength = countLines(file, fileName);
    int* indices = indexFile(file, fileName, fileLength);

    for(i = 0; i < fileLength; i++){
        printf("%d %d\n", i, indices[i]);
    }

    rewind(file);

    validateElement(file, fileName, fileLength, indices);


    free(indices);
    free(fileName);
    fclose(file);
    return 0;
}

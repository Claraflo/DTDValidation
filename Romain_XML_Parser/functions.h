#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int isXML(char* file);
int countLines(FILE * file, char * path);
int * indexFile(FILE * file, char * path, int length);



//This takes a file name and returns 1 if it sounds like a valid XML file

int isXML(char* f){
    char *suffix = ".xml";
    size_t lenFile = strlen(f);
    size_t lenSuffix = strlen(suffix);
    //entry->d_name[0] != 46

    if((lenFile > lenSuffix) && (strncmp(f + lenFile - lenSuffix, suffix, lenSuffix)) == 0){
        return 1;
    }
    return 0;
}

//Counts lines in file

int countLines(FILE* file, char * path){
    rewind(file);
    int counter = 0;
    char line[100];

    while((fscanf(file, "%[^\n]", line)) != EOF){
        fgetc(file);
        counter += 1;
    }

    return counter;

}

//This takes a file pointer and returns an array of ints corresponding to the first character of each line, to allow for random access

int* indexFile(FILE * file, char * path, int length){
    rewind(file);
    char line[100];    
    int* indices = (int*)malloc(length * sizeof(int)); 

    int i = 0;
    do{
        if(i == length) break;
        fgetc(file);
        indices[i] = (int)ftell(file);
        indices[0] = 0;
        i++;

    }while((fscanf(file, "%[^\n]", line)) != EOF);

    //indices[0] = 1;

    return indices;
}

//

void validateElement(FILE * file, char * path, int length, int* indices){
    rewind(file);
    
    char buffer[100];
    fseek(file, (long int)indices[2], SEEK_SET);
    fscanf(file, "%[^\n]", buffer);

    printf("%s\n", buffer);

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int isXML(char* file);
int countLines(FILE * file, char * path);
int * indexFile(FILE * file, char * path, int length);
char* getFileLine(int index, FILE * file, char * path, int lineCount, int* indices);



//This takes the file and makes it a string
//Should have done this before
//If only I KNEW

char* fileString(FILE * file, char* path){
    
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char * fileString = (char *)malloc(size * sizeof(char) + 1);
    
    fread(fileString, 1, size, file);
    fileString[size] = '\0';
    return fileString;
}

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

// this returns a string corresponding to the line specified by "index", 0 being the first line in the file, 1 the second etc

char* getFileLine(int index, FILE * file, char * path, int lineCount, int* indices){
    
    if(index < 0 || index >= lineCount) return NULL;

    rewind(file);
    char buffer[100];
    fseek(file, (long int)indices[index], SEEK_SET);
    fscanf(file, "%[^\n]", buffer);

    //printf("%s\n", buffer);

    char * ligne = (char*)malloc(strlen(buffer)*sizeof(char));

    strcpy(ligne, buffer);

    return ligne;
}
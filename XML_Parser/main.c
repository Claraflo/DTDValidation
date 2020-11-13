#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "file.h"
#include "structs.h"
#include "dtd.h"

/*
To do : 

detect element types & maybe make 3D matrix to house them
syntax
fix detection of newlines
*/

int main(){

    //Defining variables and directory structs
    DIR *dir;
    struct dirent *entry;
    int i = 0;

    //Checking filesystem for valid XML files
    //puts("\nLooking for XML files...");

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
        closedir(dir);
    }

    puts("");

    //User inputs desired file
    FILE *file;
    char *fileName = (char*)malloc(34 * sizeof(char));
    strcpy(fileName, "xml/");
    char input[] = "t.xml";


    //puts("\nPlease enter the name of the file:");
    while(1){
        //scanf("%s", input);
        strcat(fileName, input);
        file = fopen(fileName, "r");

        if(file != NULL && isXML(input)){
            break;
        }
        else{
            puts("File non valid.");
            memset(fileName+4,0,strlen(fileName)-4);
        }
    };
    



    // Counting lines and indexing

    int lineCount = countLines(file, fileName);
    int* indices = indexFile(file, fileName, lineCount);

    char * fileAsString = fileString(file, fileName);



    rewind(file);
    
    // The !DOCTYPE element is passed as a pointer and modified by the function
    // Same for the length
    // Any !ELEMENT elements are put into the char** array that the function returns
    int dtd_Length;
    char* doctype = NULL;
    char** elements_In_DTD = get_DTD_Elements(file, fileName, lineCount, indices, &doctype, &dtd_Length);

    node * head = link_Nodes(fileAsString, &dtd_Length, elements_In_DTD, indices);
    node * sample = head;

    while(sample->next != NULL){
        printf("%s\n", sample->keyword);
        sample = sample->next;
    }
        printf("%s\n", sample->keyword);

    printf("Doctype : %s\n", doctype);
    for(i = 0; i < dtd_Length; i++){
        printf("Element : %s\n", elements_In_DTD[i]);
    };

    //freeList(head)
    free(doctype);
    free(elements_In_DTD);
    free(indices);
    free(fileName);
    fclose(file);
    return 0;
}

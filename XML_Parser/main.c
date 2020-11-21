#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "file.h"
#include "structs.h"
#include "dtd.h"
#include "dtd.c"

/*
To do : 

vérifier validité nom éléments et charactères
détection charactères spéciaux dans éléments mais pas tag "xml version"
1ESG = segfault
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


    // // Analyze tags

    elem * firstElem = init_Elem(NULL, NULL);
    node * firstNode = init_Node(NULL);

    link_Nodes(fileAsString, indices, firstNode, firstElem, lineCount);
    
    
    node * exp = firstNode;

    puts("");
    while(exp != NULL){
        printf("%s\n", exp->keyword);
        exp = exp->next;
    }

    puts("");

    elem * temp = firstElem;
    while(temp != NULL){
        printf("%s %s\n", temp->role, temp->keyword);
        temp = temp->nextElem;
    }


    puts("");
    free_Elems(firstElem);
    free_Nodes(firstNode);
    free(indices);
    free(fileName);
    fclose(file);
    return 0;
}

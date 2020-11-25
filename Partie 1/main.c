#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "file.h"
#include "structs.h"
#include "structs.c"
#include "dtd.h"
#include "dtd.c"

int main(){
    int counter = 0;
    while(1){

    //Defining variables and directory structs
    DIR * dir;
    struct dirent * entry;
    int i = 0;

    //Checking filesystem for valid XML files
    //puts("\nLooking for XML files...");

    if((dir = opendir("xml")) == NULL){
        perror("Error opening the XML folder.");
    }
    else{
        puts("\nPlease the name of the file or send 'exit' to quit.\nFound files :\n");
        while(((entry = readdir(dir))) != NULL){
            if(isXML(entry->d_name)){
                i++;                
                printf("| %s\n", entry->d_name);
            }
        }
        closedir(dir);
    }
    counter++;

    puts("");

    //User inputs desired file
    FILE *file;
    char *fileName = (char*)malloc(34 * sizeof(char));
    strcpy(fileName, "xml/");
    char input[30];
    // char input[] = "t.xml";


    //puts("\nPlease enter the name of the file:");
        scanf("%s", input);
        if(strcmp(input, "exit") == 0){break;}
        strcat(fileName, input);
        file = fopen(fileName, "r");

        
        if(file != NULL && isXML(input)){


            // Counting lines and indexing

            int lineCount = countLines(file, fileName);
            int* indices = indexFile(file, fileName, lineCount);

            char * fileAsString = fileString(file, fileName);

            rewind(file);

            puts("");



            // // Analyze tags

            elem * firstElem = init_Elem(NULL, NULL);
            node * firstNode = init_Node(NULL);

            
            link_Nodes(fileAsString, indices, firstNode, firstElem, lineCount);
                       
            
            node * exp = firstNode;

            puts("");
            while(exp != NULL){
                printf("%s %d -%s-\n", exp->keyword, exp->index, exp->text);
                exp = exp->next;
            }

            puts("");

            elem * temp = firstElem;
            while(temp != NULL){
                printf("%s %s %s [%d %d] -> %s\n", temp->role, temp->keyword, temp->content, temp->min, temp->max, temp->parent);
                temp = temp->nextElem;
            }

            puts("\n No errors detected.\n");
            free_Elems(firstElem);
            free_Nodes(firstNode);
            free(indices);
            free(fileName);
            fclose(file);

        }
        else{
            puts("File non valid.");
            memset(fileName+4,0,strlen(fileName)-4);
        }
    }
    return 0;
}

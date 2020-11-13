#include <stdio.h>
#include <stdlib.h>
#include <string.h>

elem * get_DTD_Elements(FILE * file, char * path, int lineCount, int* indices);
int is_Valid_Element_Name(char* word);
node * link_Nodes(char * data, int * indices, elem * firstElem);
//this is test branch


void errorMessage(char * msg, int line){
    printf("%s %d\n", msg, line);
    abort();
}

// Links all the nodes into a linked list in descending order and returns the firstNode. Raises an error if element not in DTD

node * link_Nodes(char * data, int * indices, elem * firstElem){

    node * firstNode = init_Node(NULL);
    char * copy = strdup(data);

    int len = strlen(data);
    int nbfound = 0;
    int line = 1;

    // Search each char for the start of a tag
    for(int i = 0; i < len; i++){
        line = i >= indices[line+1] ? line += 1 : line;

        if(data[i] == '<'){

            char * word = strtok(&data[i+1], ">");

            //"copy" is a copy of the file needed because strtok modifies the buffer
            if(copy[i+strlen(word)+1] != '>' || word == NULL){
                errorMessage("Bracket error line", line);
            }
            else if(strspn(word, "ABDCEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_/") != strlen(word) && word != NULL){
                //if contains something else than letters or _/ (Need update, abort si charactères spéciaux mais pas si balise "xml version")
                continue;
            }

            int offset = word[0] == '/' ? 1 : 0;

            if(!is_Elem_In_DTD(firstElem, "Element", word+offset)){
                errorMessage("Keyword not found in DTD line", line);
            }            

            nbfound += 1;
            
            if(nbfound == 1){
                firstNode->keyword = word;
            }
            else{
                add_Node(firstNode, word);
            }

            //printf("%s ", word);
            i += strlen(word);
        }
    }

    return firstNode;
}


elem * get_DTD_Elements(FILE * file, char * path, int lineCount, int* indices){
    
    elem * firstElem = init_Elem(NULL, NULL);

    for(int i = 0; i < lineCount; i++){
        char * buffer = getFileLine(i, file, path, lineCount, indices);
        char * exclam = strchr(buffer, '!');

        ////recognize and tokenize : doctype and elements
        if(exclam != NULL){
            if(strncmp(exclam+1, "DOCTYPE",  7) == 0){
                char* word = strtok(exclam+9, " ");
                //if element contains special characters put it in list; otherwise exit
                if(is_Valid_Element_Name(word)){
                    firstElem->role = "Doctype";
                    firstElem->keyword = word;
                }
            }
            else if(strncmp(exclam+1, "ELEMENT",  7) == 0){
                char* word = strtok(exclam+9, " ");
                //same as above
                if(is_Valid_Element_Name(word)){

                    //checking if element is already in the list                    
                    if(is_Elem_In_DTD(firstElem, "element", word)){
                        errorMessage("Duplicate element line", i+1);
                    }
                    
                    else{
                        add_Elem(firstElem, "Element", word);
                    }
                }
                else{
                    printf("Element name invalid line %d. Exiting\n", i);
                    abort();
                }
            }
        }
    }
    return firstElem;
}


// Verifies the element name is valid according to https://www.w3schools.com/xml/xml_elements.asp
// Inefficient but it's ok
// Called in the above function

int is_Valid_Element_Name(char* word){

    if(!((word[0] == 'x' || word[0] == 'X')&&(word[1] == 'm' || word[1] == 'M')&&(word[2] == 'l' || word[2] == 'L'))
    && strspn(word, "ABDCEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.") == strlen(word)
    && strspn(word, "ABDCEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_") != 0
    ){
        return 1;
    }

    return 0;
}
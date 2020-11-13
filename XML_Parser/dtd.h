#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** get_DTD_Elements(FILE * file, char * path, int lineCount, int* indices, char** doctype, int* dtd_Length);
void count_DTD_Elements(FILE * file, char * path, int lineCount, int* indices, int* count, int* longest);
int is_Valid_Element_Name(char* word);
node * link_Nodes(char * data, int * length, char **DTD, int * indices);


// Can't u read dummy

void errorMessage(char * msg, int line){
    printf("%s %d\n", msg, line);
    abort();
}

// Links all the nodes into a linked list in descending order and returns the head. Raises an error if element not in DTD

node * link_Nodes(char * data, int * length, char **DTD, int * indices){

    node * head = init_Node(NULL);
    char * lbracketIndex = 0;
    char * rbracketIndex = 0;
    char * copy = strdup(data);

    int len = strlen(data);
    int nbfound = 0;
    int line = 1;

    for(int i = 0; i < len; i++){
        line = i >= indices[line+1] ? line += 1 : line;

        if(data[i] == '<'){

            lbracketIndex = &data[i];
            char * word = strtok(lbracketIndex+1, ">");

            if(copy[i+strlen(word)+1] != '>' || word == NULL){
                errorMessage("Bracket error line", line);
            }
            else if(strspn(word, "ABDCEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_/") != strlen(word) && word != NULL){
                continue;
            }

            int offset = word[0] == '/' ? 1 : 0;
            int found_In_DTD = 0;

            for(int j = 0; j < *length; j++){
                if(strcmp(DTD[j], word+offset) == 0){
                    found_In_DTD = 1;
                    break;
                }
            }
            nbfound += 1;

            if(found_In_DTD == 0){
                errorMessage("Keyword not found in DTD line", line);
            }

            if(nbfound == 1){
                head->keyword = word;
            }
            else{
                add_Node(head, word);
            }

            //printf("%s ", word);
            i += strlen(word);
        }
    }

    return head;
}


char** get_DTD_Elements(FILE * file, char * path, int lineCount, int* indices, char** doctype, int* dtd_Length){
    int count, longest;

    count_DTD_Elements(file, path, lineCount, indices, &count, &longest);
    *dtd_Length = count;
    
    char ** elements_In_DTD = (char**)malloc(count * sizeof(char*));

    for(int i = 0; i < count; i++){
        elements_In_DTD[i] = (char*)calloc(longest, sizeof(char));
    }

    //printf("%d %d\n", count, longest);

    int inserted = 0;
    for(int i = 0; i < lineCount; i++){
        char * buffer = getFileLine(i, file, path, lineCount, indices);
        char * exclam = strchr(buffer, '!');

        //recognize and tokenize : doctype and elements
        if(exclam != NULL){
            if(strncmp(exclam+1, "DOCTYPE",  7) == 0){
                char* word = strtok(exclam+9, " ");
                *doctype = realloc(*doctype, strlen(word));
                strcpy(*doctype, word);
            }
            else if(strncmp(exclam+1, "ELEMENT",  7) == 0){
                char* word = strtok(exclam+9, " ");

                //if element contains special characters put it in array; otherwise exit
                if(is_Valid_Element_Name(word)){
                    int duplicate = 0;

                    //checking if element is already in the list
                    for(int e = 0; e < inserted; e++){
                        if(strcmp(elements_In_DTD[e], word) == 0){
                            duplicate = 1;
                        }
                    }

                    if(duplicate == 1){
                        printf("Duplicate element found line %d\n", i);
                        abort();
                    }
                    else{
                        strcpy(elements_In_DTD[inserted], word);
                        inserted += 1;
                    }
                }
                else{
                    printf("Element name invalid line %d. Exiting\n", i);
                    abort();
                }
            }
        }
    }
    return elements_In_DTD;
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


// Counts dtd elements for array creation purposes. Is called in get_DTD_Elements

void count_DTD_Elements(FILE * file, char * path, int lineCount, int* indices, int* count, int* longest){
    int total = 0;
    int length = 0;

    for(int i = 0; i < lineCount; i++){

        char * buffer = getFileLine(i, file, path, lineCount, indices);
        char * exclam = strchr(buffer, '!');

        //count the number of lines with "!"
        if(exclam != NULL && strncmp(exclam+1, "ELEMENT",  7) == 0){
            int wordLen = strlen(strtok(exclam+9, " "));
            total += 1;
            length = length < wordLen ? wordLen : length;
        }
    }

    *count = total;
    *longest = length;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errorMessage(char * msg, int line){
    if(line != 0){
        printf("%s %d\n", msg, line);
        exit(0);
    }
    else{
        printf("%s\n", msg);
        //abort();
    }
}

// Links all the nodes into a linked list in descending order and returns the firstNode. Raises an error if element not in DTD

node * link_Nodes(char * data, int * indices, node * firstNode, elem * firstElem, int lineCount){

    char * copy = strdup(data);

    int len = strlen(data);
    int nbfound = 0;
    int line = 1;
    int isDTDfound = 0;
    int offset = 0;
    int nested = 0; // How deep
    int index = 0;  // How many tags in total
    int lastonewasanopeningtag = 0;
    char * ptr;

    // Search each char for the start of a tag
    for(int i = 0; i < len; i++){
        line = i >= indices[line+1] ? line + 1 : line;

        if(data[i] != '<' && data[i] != '\n'){
            if(lastonewasanopeningtag){
                printf(".%c.%d.%d\n", copy[i], lastonewasanopeningtag, nested);
            }
            else{
                errorMessage("Invalid character line", findLine(i, indices, lineCount));
                break;
            }
        }
        else{
            char * word = strtok_r(&data[i+1], ">\n", &ptr);
            int j = i;
            i += strlen(word)+1;


            if(word[1] == ' ' || (word[1] == '!' && isDTDfound == 1)){
                //printf("^%c^\n", copy[i]);
                errorMessage("Invalid element name line", line+1);
            }

            //DTD elements
            else if(word[1] == '!' && isDTDfound == 0){
                get_DTD_Elements(copy, &j, firstElem, indices, lineCount);
                isDTDfound = 1;
                i = j;
                continue;
            }

            offset = word[0] == '/' ? 1 : (word[0] == '<' ? (word[1] == '/' ? 2 : 1) : 0); //is word prefixed by / or < or </

            //"copy" is a copy of the file needed because strtok modifies the buffer
            if(copy[j+strlen(word)+1] != '>' || word == NULL){
                errorMessage("Bracket error line", findLine(i, indices, lineCount));
            }
            else if(strspn(word+offset, "ABDCEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_/=") != strlen(word+offset) && word != NULL){
                //if contains something else than letters or _/ (Need update, abort si charactères spéciaux mais pas si balise "xml version")
                continue;
            }


            if(firstElem->keyword != NULL && !is_Elem_In_DTD(firstElem, "Element", word+offset)){
                errorMessage("Keyword not found in DTD line", findLine(i, indices, lineCount));
            }            

            nbfound += 1;
            
            offset = word[0] == '<' ? 1 : 0;
            if(nbfound == 1){
                firstNode->keyword = word+offset;
            }
            else{
                add_Node(firstNode, word+offset);
            }

            if(word[0+offset] != '/'){ nested += 1; lastonewasanopeningtag = 1;}
            else if(word[0+offset] == '/'){ nested -= 1; lastonewasanopeningtag = 0;}

        }
    }

    free(copy);
    return firstNode;
}



void get_DTD_Elements(char * data, int * position, elem * firstElem, int * indices, int lineCount){
    int i = *position;
    char * copy = strdup(data);
    //printf("%s", copy);

    char * word = strtok(&data[i+2], "[");
    i += strlen(word) + 2;

        if(strncmp(word+1, "DOCTYPE",  7) == 0){
            char * rank = strtok(word, " ");
            char * value = strtok(NULL, " ");
            if(value == NULL){
                
                errorMessage("Incorrect element name line", findLine(*position, indices, lineCount));
            }

            //if element contains special characters put it in list; otherwise exit
            if(is_Valid_Element_Name(rank, value)){
                firstElem->role = "Doctype";
                firstElem->keyword = value;
            }
            if(copy[i-1] == '>'){
                printf("DTD analyzed.\n");
                return;
            }
            else if(copy[i] == '['){
                ignoreSpaces(&copy[0], &i);
                if(copy[i] == ']'){
                    printf("DTD analyzed.\n");
                    return;
                }
            }

            ignoreSpaces(&copy[0], &i);

            char * pBracket = strchr(&copy[i-1], (int)']');
            if(pBracket == NULL){
                errorMessage("DTD unclosed", 0);
            }
            else{
                char * newdata = strdup(&copy[i-1]);
                char * token = strtok(newdata, "]");
                dtd_Tags(token, firstElem, &i, indices, lineCount);

                if(!(pBracket[1] == '>' ||((pBracket[1] == ' ' || pBracket[1] == '\n') && pBracket[2] == '>'))){
                    errorMessage("DTD unclosed", 0);
                    return;}
            }
        }
    *position = i; // Advance i for the link_nodes function
}


void dtd_Tags(char * data, elem * firstElem, int * position, int * indices, int lineCount){
    char * copy = strdup(data);
    char * temp, * ptr; 
    int i = 0;
    int j = 0;
    int len = 0;
    int stage = 0; //stage is which word is being examined in the <> tag
    int element = 0; //1 is "ELEMENT", 2 is "ATTLIST"

    if(copy[i] == '<' && copy[i+1] == '!'){
        temp = strtok_r(data, " >\n", &ptr);
        len = strlen(temp);

        i = copy[i + len+1] == 0xa ? i + len+2 : i + len+1;

        if(!strncmp(temp+2, "ELEMENT", 7)){
            stage = 1;
            element = 1;
            //printf(" %d .%s. `%c`\n", stage, temp, copy[i]);
        }
        else if(!strncmp(temp+2, "ATTLIST", 7)){
            stage = 1;
            element = 2;

            //printf(".%s. `%c`\n", temp, copy[i]);
            puts("ATTLIST not handled yet !");
            return;
        }
        else return;
    }
    else return;

    while(temp != NULL){
        temp = strtok_r(NULL, " >\n", &ptr);
        if(temp == NULL) break;

        len = strlen(temp);
        int e = i;
        j = i + len;
        i = copy[i + len + 1] == 0xa ? i + len+2 : i + len+1;

        if(!strncmp(temp+2, "ELEMENT", 7)){
            element = 1;
        }
        else if(!strncmp(temp+2, "ATTLIST", 7)){
            element = 2;
        }

        //!Element
        if(element == 1){
            if(stage == 3){stage = 1;} else stage++; //Circle back to 1

            if(stage == 1 && !((temp[0] != '<' || temp[1] != '!') || (copy[j] != ' ' && copy[j] != '\n'))){ // is <!ELEMENT correctly formed ? if yes :

            }
            else if(stage == 2 && is_Valid_Element_Name(NULL, temp) && (copy[j] == ' ' || copy[j] == '\n')){
                //checking if element is already in the list                    
                if(is_Elem_In_DTD(firstElem, "element", temp)){
                    errorMessage("Duplicate element in DTD", findLine(*position+e, indices, lineCount));
                }     
                else{
                    add_Elem(firstElem, "Element", temp);
                }
            }
            else if(stage == 3 && temp[0] == '(' && copy[j-1] == ')'){
                int offset = 0;
                if(copy[j] == 0x20 || copy[j] == '\n'){
                    i+=2;
                    j+=2;
                    offset++;
                }
                if(copy[j-offset] != '>'){
                    errorMessage("Line unclosed in DTD", findLine(*position+e, indices, lineCount));
                    //printf("{%c}", copy[j]);
                    //return;                  
                }
            }
            else{
                errorMessage("Incorrect element in DTD", findLine(*position+e, indices, lineCount));
                return;
            }
            //printf(" %d ", stage);
        }
        //!Attlist
        else if(element == 2){
            //if(stage == 4){stage = 1;} else stage++;
            puts("ATTLISTS not handled yet !");
            return;
        }
        else return;

        //printf(".%s. -%c- [%c%c]\n", temp, copy[j], copy[i], copy[i+1]);
    }
    *position += i; // Advance i for the link_nodes function
}


// Advances i by 1, and again until there are no spaces

void ignoreSpaces(char * data, int * i){
    int j = *i+1;
    while(data[j] == 0x20 || data[j] == 0x10|| data[j] == 0xa){
        //printf("Space ignored.\n");
        j += 1;
    }
    *i = j;
}

// Verifies the element name is valid according to https://www.w3schools.com/xml/xml_elements.asp
// Inefficient but it's ok
// Called in the above function

int is_Valid_Element_Name(char * rank, char* value){

    if(!((value[0] == 'x' || value[0] == 'X')&&(value[1] == 'm' || value[1] == 'M')&&(value[2] == 'l' || value[2] == 'L'))
    && strspn(value, "ABDCEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.") == strlen(value)
    && strspn(value, "ABDCEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_") != 0
    ){
        return 1;
    }

    return 0;
}
////////////////////////////////////////////
//NODES

void free_Nodes(node * original);
node * init_Node(char * data);
int countOccurences(node * firstNode, char * keyword);
void add_Node(node * original, char * data);
int canInsertText(elem * original, char * keyword);
void write_Node(node * original, int index, char * data);


// Adds a node in the linked list

void add_Node(node * original, char * data){

    node * temp = original;
    int index = 1;

    while(temp->next != NULL){
        temp = temp->next;
        index++;
    }

    node * new = init_Node(data);

    temp->next = new;
    new->index = index;
    new->previous = temp;
}

// Creates node and init variables

node * init_Node(char * data){
    node * unit = malloc(sizeof(node));

    unit->index = 0;
    unit->text[0] = '\0';
    unit->keyword = data;

    unit->next = NULL;
    unit->previous = NULL;

    return unit;
}

// Writes text into the node at [index]

// void write_Node(node * original, int index, char * data){

//     node * temp = original;

//     while(temp->next != NULL && temp->index != index){
//         temp = temp->next;
//     }

//     temp->text = data;
// }

// Frees entire node list
void free_Nodes(node * original){
    node * temp = original;

    while(temp != NULL){
        node * next = temp->next;
        free(temp);
        temp = next;
    }
}

int canInsertText(elem * original, char * keyword){
    elem * temp = original;

    while(temp != NULL){
        temp = temp->nextElem;
        if(strcmp(temp->keyword, keyword) == 0){
            break;
        }
    }

    if(strcmp(temp->content, "#PCDATA") == 0){
        return 1;
    }

    return 0;
}

int countOccurences(node * firstNode, char * keyword){
    node * temp = firstNode;
    int i = 0;

    while(temp != NULL){
        if(strcmp(temp->keyword, keyword) == 0){
            i++;
        }
        temp = temp->next;
    }

    return i;
}






/////////////////////////////////////////////
//ELEMS


elem * init_Elem(char * rank, char * data);
elem * verifyOccurences(elem * firstElem, node * firstNode, int * count);
int labelElem(elem * original, char * rank, char * word, char * content);
void add_Elem(elem * original, char * rank, char * data, char * parent, int min, int max);
int is_Elem_In_DTD(elem * original, char * rank, char * word);
int checkOrder(node * integrityNode, elem * firstElem);
void free_Elems(elem * original);


elem * init_Elem(char * rank, char * data){
    elem * unit = malloc(sizeof(elem));

    unit->role = rank;
    unit->keyword = data;
    
    unit->min = 0;
    unit->max = 0;
    unit->content = NULL;
    unit->previousElem = NULL;
    unit->nextElem = NULL;
    unit->nextAttrib = NULL;

    return unit;

}

void add_Elem(elem * original, char * rank, char * data, char * parent, int min, int max){
    elem * temp = original;

    while(temp->nextElem != NULL){

        temp = temp->nextElem;
    }

    elem * new = init_Elem(rank, data);

    temp->nextElem = new;

    new->previousElem = temp;
    new->parent = parent;
    new->min = min;
    new->max = max;
}

// Returns 1 if element is found in DTD
int is_Elem_In_DTD(elem * original, char * rank, char * word){
    elem * temp = original;
    int counter = 0;    

    while(temp != NULL){
        if(strcmp(temp->keyword, word) == 0 && strcmp(temp->role, rank) == 0){
            return 1;
        }

        temp = temp->nextElem;
    }

    return 0;
}

//frees entire element list
//does not include attributes yet
void free_Elems(elem * original){
    elem * temp = original;

    while(temp != NULL){
        elem * next = temp->nextElem;
        free(temp);
        temp = next;
    }
}

// Labels elems as PCDATA when called

int labelElem(elem * original, char * rank, char * word, char * content){
    elem * temp = original;

    while(temp != NULL){
        if(strcmp(temp->role, rank) == 0 && strcmp(temp->keyword, word) == 0){
            break;
        }
        temp = temp->nextElem;
    }

    if(temp->content == NULL){
        temp->content = content;
    }
    else{
        return 0;
    }
    return 1;
}

// Returns an elem so that error message display min / max

elem * verifyOccurences(elem * firstElem, node * firstNode, int * count){
    elem * elem1 = firstElem;
    node * node1 = firstNode;

    while(elem1 != NULL){
        int num = countOccurences(firstNode, elem1->keyword);
        printf("%d number of node %s\n", num, elem1->keyword);
        if(num < elem1->min || num > elem1->max){
            *count = num;
            return elem1;
        }

        elem1 = elem1->nextElem;
    }
    return NULL;
}


// CheckOrder takes the first node to a list where tags are automatically stored.
// When an element is closed in the XML, the opening and closing tags are both removed from the list, leaving only unclosed tags
// Example : <firstex><ex></ex> the ex tags cancel out
// CheckOrder also checks whether an opened tag has business business being opened

int checkOrder(node * integrityNode, elem * firstElem){
    node * integrity = integrityNode;


    while(integrity->next != NULL){
        integrity = integrity->next;
    }

    char * word = integrity->keyword;

    //if it's a closing bracket
    if(word[0] == '/'){
        if(integrity->previous == NULL){
            return 0;
        }

        node * previousInteg = integrity->previous;  
        char * previousWord = previousInteg->keyword;

        if(previousWord[0] != '/' && strcmp(previousWord, &word[1]) == 0){
                
                // puts("REMOVING");
                if(previousInteg->previous != NULL){
                    node * last = previousInteg->previous;
                    last->next = NULL; 
                }

                previousInteg->keyword = NULL;
                previousInteg->next = NULL;
                previousInteg = NULL;
                free(previousInteg);

                // integrity->keyword = NULL;
                integrity = NULL;
                free(integrity);

        }

        if(integrityNode->keyword){
            char * test = integrityNode->keyword;
            if(test[0] == '/'){return 0;}
        }
    }
    // If it's an opening bracket
    else{
        elem * tempElem = firstElem;
        while(tempElem->nextElem != NULL){
            if (strcmp(tempElem->keyword, integrity->keyword) == 0){
                break;
            }
            tempElem = tempElem->nextElem;
        }

        // If the last element has a parent, and the DTD allows it
        if(integrity->previous != NULL && tempElem->parent != NULL){
            node * previousInteg = integrity->previous;  
            char * previousWord = previousInteg->keyword;

            // If the parent, and the expected parent don't match
            if(strcmp(tempElem->parent, previousInteg->keyword) != 0 ){
                return 0;
            }
        }
        // If the last element has no parent, but the DTD says it should
        else if (integrity->previous == NULL && tempElem->parent != NULL){
            return 0;
        }
    }

    return 1;
}
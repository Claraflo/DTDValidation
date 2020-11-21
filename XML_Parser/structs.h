#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_s node;
typedef struct elem_s elem;
typedef struct attrib_s attrib;
typedef struct type_s type;

// Nodes are elements surrounded by < > outside the DTD
struct node_s{
    char * keyword;
    char * text;

    struct node_s *previous;
    struct node_s *next;
};

// Role can be "Doctype" or "Element"
struct elem_s{
    char * role;
    char * keyword;

    struct elem_s *nextElem;
    struct attrib_s *nextAttrib;
};

struct attrib_s{
    char * keyword;

    struct attrib_s *nextAttrib;
};


/////////////////////////////////////////////
//NODES

node * init_Node(char * data);
void add_Node(node * original, char * data);


// Adds a node in the linked list

void add_Node(node * original, char * data){

    node * temp = original;

    while(temp->next != NULL){
        temp = temp->next;
    }

    node * new = init_Node(data);

    temp->next = new;
    new->previous = temp;
}

// Creates node and init variables

node * init_Node(char * data){
    node * unit = malloc(sizeof(node));

    unit->keyword = data;
    unit->next = NULL;
    unit->previous = NULL;

    return unit;
}

void write_Node(node * original, int index, char * data){

    node * temp = original;

    while(temp->next != NULL){
        temp = temp->next;
    }

    node * new = init_Node(data);

    temp->next = new;
    new->previous = temp;
}

// Frees entire node list
void free_Nodes(node * original){
    node * temp = original;

    while(temp != NULL){
        node * next = temp->next;
        free(temp);
        temp = next;
    }
}

/////////////////////////////////////////////
//ELEMS


elem * init_Elem(char * rank, char * data);
void add_Elem(elem * original, char * rank, char * data);
int is_Elem_In_DTD(elem * original, char * rank, char * data);
void free_Elems(elem * original);


elem * init_Elem(char * rank, char * data){
    elem * unit = malloc(sizeof(elem));

    unit->role = rank;
    unit->keyword = data;
    
    unit->nextElem = NULL;
    unit->nextAttrib = NULL;

    return unit;

}

void add_Elem(elem * original, char * rank, char * data){
    elem * temp = original;

    while(temp->nextElem != NULL){

        temp = temp->nextElem;
    }

    elem * new = init_Elem(rank, data);

    temp->nextElem = new;
}

// Returns 1 if element is found in DTD
int is_Elem_In_DTD(elem * original, char * rank, char * word){
    elem * temp = original;

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_s node;

struct node_s{
    char * keyword;

    struct node_s *previous;
    struct node_s *next;
};

node * init_Node(char * data);


// Adds a node in the linked list

void add_Node(node * original, char * data){

    node * head = original;

    while(head->next != NULL){
        head = head->next;
    }

    node * new = init_Node(data);

    head->next = new;
    new->previous = head;
}

// Create node and init variables

node * init_Node(char * data){
    node * unit = malloc(sizeof(node));

    unit->keyword = data;
    unit->next = NULL;
    unit->previous = NULL;

    return unit;
}
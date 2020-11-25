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
    char text[30];
    int index;

    struct node_s *previous;
    struct node_s *next;
};

// Role can be "Doctype" or "Element"
struct elem_s{
    char * parent;
    char * role;
    char * keyword;
    char * content; // can be #PCDATA
    int min;
    int max;

    struct elem_s *previousElem;
    struct elem_s *nextElem;
    struct attrib_s *nextAttrib;
};


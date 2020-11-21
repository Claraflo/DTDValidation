void get_DTD_Elements(char * data, int * position, elem * firstElem, int * indices, int lineCount);
node * link_Nodes(char * data, int * indices, node * firstNode, elem * firstElem, int lineCount);
void dtd_Tags(char * data, elem * firstElem, int * position, int * indices, int lineCount);
int is_Valid_Element_Name(char * rank, char* word);
void ignoreSpaces(char * data, int * i);

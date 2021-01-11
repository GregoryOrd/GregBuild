#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// ****************************************************************************************************//
// In an attempt to make this linked list structure generic, the data inside of the Node structure   **//
// is simply a void pointer.                                                                         **//
// Currently the mechanism for type protection is very loose. The linked list has an integer member  **//
// that the user can use to assign a type to the data of the linked list.                            **//
// Each linked list function then requires an integer argument representing this type to be passed   **//
// in to allow the execution of the function. The decision of which integer represents which data    **//
// type will be left to the user of the linked list and should be consistent through multiple uses   **//
// of the linked list in their application.                                                          **//
// ****************************************************************************************************//

typedef struct Node
{
    void* data;
    struct Node* next;
} Node;

typedef struct LinkedList
{
    int dataType;
    int size;
    Node* head;
} LinkedList;

void initEmptyLinkedList(LinkedList* list, int type);
void freeLinkedList(LinkedList* list);

// These functions will return -1 on failure
int size_ll(LinkedList* list, int type);
int type_ll(LinkedList* list);
int insert_ll(LinkedList* list, void* data, int type, int index);
int append_ll(LinkedList* list, void* data, int type);

// These functions will return a null pointer on failure
const void* at_ll(LinkedList* list, int type, int index);
const void* last_ll(LinkedList* list, int type);

#endif
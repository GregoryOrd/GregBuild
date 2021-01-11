#include "LinkedList.h"

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
//              Private Function Prototypes                         //
//////////////////////////////////////////////////////////////////////
int checkType(LinkedList* list, int type);
int checkIndex(LinkedList* list, int index);


void main()
{
    printf("Running Linked List Test\n");
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    printf("Allocated Memory for List\n");
    initEmptyLinkedList(list, 1);
    printf("Initialized Empty List\n");
    freeLinkedList(list);
    printf("Freed Linked List\n");
}

//////////////////////////////////////////////////////////////////////
//              Function Implementation Section                     //
//////////////////////////////////////////////////////////////////////

int checkType(LinkedList* list, int type)
{
    if(list->dataType != type)
    {
        printf("Linked List Type Error. Can not use type: %d for Linked List of type: %d\n", 
            type, list->dataType);
        return 1;
    }
    return 0;
}

int checkIndex(LinkedList* list, int index)
{
    if(index >= list->size)
    {
        printf("Linked List Index Out of Bounds. Can not access index: %d for Linked List of size: %d\n", index, list->size);
        return 1;
    }
    
    if(index < 0)
    {
        printf("Linked List Index Out of Bounds. Can not access index less than or equal to zero.\n", index, list->size);
        return 1;
    }
    return 0;
}

//This function assumes the linked list has already been allocated memory dynamically
void initEmptyLinkedList(LinkedList* list, int type)
{
    list->dataType = type;
    list->size = 0;
    list->head = 0;
}

void freeLinkedList(LinkedList* list)
{
    Node* itr = list->head;
    Node* prev = itr;
    while(itr != 0)
    {
        itr = itr->next;
        free(prev->data);
        prev->next = 0;
        prev = itr;
    }
    list->dataType = 0;
    list->size = 0;
}

int size_ll(LinkedList* list, int type)
{
    int typeError = checkType(list, type);
    if(!typeError)
    {
        return list->size;
    }
    return -1;
}

int type_ll(LinkedList* list)
{
    return list->dataType;
}

int insert_ll(LinkedList* list, void* data, int type, int index){}
int append_ll(LinkedList* list, void* data, int type){}
const void* at_ll(LinkedList* list, int type, int index){}
const void* last_ll(LinkedList* list, int type){}




#include "Collections.h"
#include <stdlib.h>

char *ListGetElement(const List *list, const size_t index)
{
    return ((char *)list->Body) + ((index + list->Offset) % list->Length) * list->ElementSize;
}

int ListCreate(const size_t elementSize, const size_t startingLength, List *listDest)
{
    List list;

    list.ElementSize = elementSize;
    list.Count = 0;
    list.Offset = 0;
    list.Length = startingLength;
    list.Body = malloc(list.Length * list.ElementSize);

    if(list.Body == NULL)
        return 0;

    *listDest = list;
    return 1;
}

int ListResize(List *list, const size_t length)
{
    list->Length = length;
    void *tempBody = realloc(list->Body, length * list->ElementSize);

    if(tempBody == NULL)
        return 0;
    
    list->Body = tempBody;
    return 1;
}

void ListSet(List *list, const size_t index, const void *element)
{
    char *listElement = ListGetElement(list, index);

    for(size_t x = 0; x < list->ElementSize; x++)
        listElement[x] = ((char *)element)[x];
}

void ListGet(const List *list, const size_t index, void *elementDest)
{
    const char *listElement = ListGetElement(list, index);

    for(size_t x = 0; x < list->ElementSize; x++)
        ((char *)elementDest)[x] = listElement[x];
}

int ListAdd(List *list, const void *element)
{
    if(list->Count >= list->Length && !ListResize(list, (list->Count * 2 / 3 + (list->Count == 0)) * list->ElementSize))
        return 0;

    ListSet(list, list->Count, element);
    return 1;
}

void ListRemove(List *list, const size_t index)
{
    list->Count--;

    if(index == 0)
    {
        list->Offset++;
        return;
    }

    char *listElement = ListGetElement(list, index);
    char *nextListElement = listElement + list->ElementSize;

    for(size_t x = 0; x < list->ElementSize * (list->Count - index - 1); x++)
        listElement[x] = nextListElement[x];
}

void ListPopEnd(List *list, void *elementDest)
{
    ListGet(list, list->Count - 1, elementDest);
    list->Count--;
}

void ListPopFront(List *list, void *elementDest)
{
    ListGet(list, 0, elementDest);
    list->Count--;
    list->Offset++;
}

void ListFree(List *list)
{
    free(list->Body);
}
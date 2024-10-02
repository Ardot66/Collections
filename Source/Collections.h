#ifndef __COLLECTIONS__
#define __COLLECTIONS__

#include "stdlib.h"

typedef size_t ValidityListNumber;

typedef struct List List;
typedef struct Dictionary Dictionary;
typedef struct DStack DStack;

struct List
{
    void *Body;
    size_t ElementSize;
    size_t Length;
    size_t Count;
    size_t Offset;
};

struct Dictionary
{
    void *Body;
    ValidityListNumber *ValidityList;
    size_t KeySize;
    size_t ValueSize;
    size_t Length;
    size_t Count;
    
    size_t (*HashFunction)(void *key);
};

struct DStack
{
    void *Body;
    ValidityListNumber *ValidityList;
    size_t ElementSize;
    size_t Length;
    size_t Count;
    size_t RemovedCount;
};

int ListCreate(const size_t elementSize, const size_t startingLength, List *listDest);
int ListResize(List *list, const size_t length);
void ListSet(List *list, const size_t index, const void *element);
void ListGet(const List *list, const size_t index, void *elementDest);
int ListAdd(List *list, const void *element);
void ListRemove(List *list, const size_t index);
void ListPopEnd(List *list, void *elementDest);
void ListPopFront(List *list, void *elementDest);
void ListFree(List *list);

#endif
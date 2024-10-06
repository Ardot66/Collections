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
    
    size_t (*HashFunction)(const void *key);
    int (*EqualityFunction)(const void *a, const void *b);
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

int ArrayResize(void **array, size_t *length, const size_t elementSize, const size_t newLength);
void ArrayInsert(void *array, size_t *count, const size_t elementSize, const size_t index, const void *element);
void ArrayRemove(void *array, size_t *count, const size_t elementSize, const size_t index);

void *CArrayGetElement(const void *array, const size_t length, const size_t offset, const size_t elementSize, const size_t index);
void CArraySet(void *array, const size_t length, const size_t offset, const size_t elementSize, const size_t index, const void *element);
void CArrayGet(const void *array, const size_t length, const size_t offset, const size_t elementSize, const size_t index, void *elementDest);
void CArrayResizeElements(void *array, const size_t count, const size_t length, size_t *arrayOffset, const size_t elementSize, const size_t newLength);
int CArrayResize(void **array, const size_t count, size_t *length, size_t *arrayOffset, const size_t elementSize, const size_t newLength);
void CArrayInsert(void *array, size_t *count, const size_t length, size_t *offset, const size_t elementSize, const size_t index, const void *element);
void CArrayRemove(void *array, size_t *count, const size_t length, size_t *offset, const size_t elementSize, const size_t index);

#endif
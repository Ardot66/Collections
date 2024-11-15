#ifndef __COLLECTIONS__
#define __COLLECTIONS__

#include <stddef.h>

// Tries to resize an array if count is larger than or equal to length.
// Resizer should increase the size of the array and produce an int result.
// Any code in failHandler will only run if the result from resizer is non-zero.
#define ARRAY_GENERIC_TRY_RESIZE(count, length, resizer, failHandler)\
{\
    if((count) >= (length))\
    {\
        int result = resizer;\
\
        if(result)\
        {\
            failHandler\
        }\
    }\
}

// Sets an element in an array at a specific index.
// The element passed should be a void*.
// Getter should produce a arrayType* to the element in the array at the index specified by the size_t variable [getIndex].
#define ARRAY_GENERIC_SET(arrayType, elementSize, index, element, getter)\
{\
    const size_t scaledElementSize = elementSize / sizeof(arrayType);\
    size_t getIndex = index * scaledElementSize;\
\
    for(size_t x = 0; x < scaledElementSize; x++, getIndex++)\
        *(arrayType *)(getter) = *((arrayType *)element + x);\
}

// Moves a segment of an array starting at [index] that is [count] indices long [shift] indices to the right.
// Getter should produce a arrayType* to the element in the array at the index specified by the size_t variable [getIndex].
#define ARRAY_GENERIC_RIGHTSHIFT(arrayType, elementSize, shift, index, count, getter) \
{\
    const size_t scaledElementSize = elementSize / sizeof(arrayType);\
    size_t getIndex;\
\
    for(getIndex = (index + count) * scaledElementSize; getIndex > (index) * scaledElementSize; getIndex--)\
    {\
        arrayType *to = (getter);\
        getIndex -= scaledElementSize * (shift);\
        arrayType *from = (getter);\
        getIndex += scaledElementSize * (shift);\
        *to = *from;\
    }\
}

// Moves a segment of an array starting at [index] that is [count] indices long [shift] indices to the left.
// Getter should produce a arrayType* to the element in the array at the index specified by the size_t variable [getIndex].
#define ARRAY_GENERIC_LEFTSHIFT(arrayType, elementSize, shift, index, count, getter)\
{\
    const size_t scaledElementSize = elementSize / sizeof(arrayType);\
    size_t getIndex;\
\
    for(getIndex = (index) * scaledElementSize; getIndex < ((count) + (index)) * scaledElementSize; getIndex++)\
    {\
        getIndex -= scaledElementSize * (shift);\
        arrayType *to = (getter);\
        getIndex += scaledElementSize * (shift);\
        arrayType *from = (getter);\
        *to = *from;\
    }\
}

typedef unsigned int ExistsListNum;
typedef size_t (*HashFunction)(const size_t keySize, const void *key);
typedef int (*EqualityFunction)(const size_t size, const void *a, const void *b);

int ArrayResize(void **array, size_t *length, const size_t elementSize, const size_t newLength);
int ArrayInsert(void **array, size_t *count, size_t *length, const size_t elementSize, const size_t index, const void *element);
void ArrayRemove(void *array, size_t *count, const size_t elementSize, const size_t index);

typedef struct CArray CArray;
struct CArray
{
    void *Body;
    size_t Length;
    size_t Count;
    size_t Offset;
};

#define CArrayGet(cArray, elementSize, index) ((void *)(((char *)(cArray)->Body) + ((index + (cArray)->Offset) % (cArray)->Length) * elementSize))

int CArrayResize(CArray *cArray, const size_t elementSize, const size_t newLength);
int CArrayInsert(CArray *cArray, const size_t elementSize, const size_t index, const void *element);
void CArrayRemove(CArray *cArray, const size_t elementSize, const size_t index);

size_t DictGetBodySize(const size_t length, const size_t keySize, const size_t valueSize);
size_t DictGetExistsListSize(const size_t length);
void DictSetup(ExistsListNum *existsList, const size_t length);
int DictAllocate(ExistsListNum **existsListDest, void **bodyDest, const size_t length, const size_t keySize, const size_t valueSize);
void *DictGetKey(void *body, const size_t keySize, const size_t valueSize, const size_t index);
void *DictGetValue(void *body, const size_t keySize, const size_t valueSize, const size_t index);
int DictIndexOf(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const void *key, EqualityFunction keyEqualityFunction, size_t *indexDest);
int DictAdd(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const void *key, const void *value);
int DictRemove(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const size_t index);
int DictIterate(ExistsListNum *existsList, const size_t length, const size_t startIndex, size_t *nextElementIndexDest);
int DictResize(ExistsListNum **existsList, void **body, size_t *length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const size_t newLength);
size_t DictDefaultHash(const size_t keySize, const void *key);
int DictDefaultEquate(const size_t keySize, const void *keyA, const void *keyB);
void DictFree(void *body);

#endif
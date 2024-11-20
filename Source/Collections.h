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
    for(getIndex = ((index) + (count)) * scaledElementSize - 1; getIndex >= (index) * scaledElementSize & getIndex != SIZE_MAX; getIndex--)\
    {\
        getIndex += scaledElementSize * (shift);\
        arrayType *to = (getter);\
        getIndex -= scaledElementSize * (shift);\
        arrayType *from = (getter);\
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

typedef struct Dictionary Dictionary;
struct Dictionary
{
    size_t Length;
    size_t Count;
    void *Body;
};

static inline void *DictGetKey(const Dictionary *dictionary, const size_t keySize, const size_t valueSize, const size_t index)
{
    return ((char *)dictionary->Body) + index * (keySize + valueSize);
}

static inline void *DictGetValue(const Dictionary *dictionary, const size_t keySize, const size_t valueSize, const size_t index)
{
    return ((char *)dictionary->Body) + index * (keySize + valueSize) + keySize;
}

size_t DictGetSize(const size_t length, const size_t keySize, const size_t valueSize);
void DictInit(Dictionary *dictionary, const size_t keySize, const size_t valueSize);
int DictIndexOf(const Dictionary *dictionary, const size_t keySize, const size_t valueSize, const HashFunction hashFunction, const EqualityFunction keyEqualityFunction, const void *key, size_t *indexDest);
int DictResize(Dictionary *dictionary, const size_t keySize, const size_t valueSize, const HashFunction hashFunction, const size_t newLength);
int DictAdd(Dictionary *dictionary, const size_t keySize, const size_t valueSize, const HashFunction hashFunction, const size_t resizePercentage, const void *key, const void *value);
void DictRemove(Dictionary *dictionary, const size_t keySize, const size_t valueSize, const HashFunction hashFunction, const size_t index);
int DictIterate(const Dictionary *dictionary, const size_t keySize, const size_t valueSize, size_t *index);
size_t DictDefaultHash(const size_t keySize, const void *key);
int DictDefaultEquate(const size_t keySize, const void *keyA, const void *keyB);

#endif
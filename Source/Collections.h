#ifndef __COLLECTIONS__
#define __COLLECTIONS__

#include "stdlib.h"

typedef unsigned int ExistsListNum;
typedef size_t (*HashFunction)(const size_t keySize, const void *key);
typedef int (*EqualityFunction)(const size_t size, const void *a, const void *b);

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

size_t DictGetBodySize(const size_t length, const size_t keySize, const size_t valueSize);
size_t DictGetExistsListSize(const size_t length);
int DictAllocate(ExistsListNum **existsListDest, void **bodyDest, const size_t length, const size_t keySize, const size_t valueSize);
void DictSetElementExists(ExistsListNum *existsList, const size_t index, const int exists);
void *DictGetKey(void *body, const size_t keySize, const size_t valueSize, const size_t index);
void *DictGetValue(void *body, const size_t keySize, const size_t valueSize, const size_t index);
int DictIndexOf(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const void *key, EqualityFunction keyEqualityFunction, size_t *indexDest);
int DictAdd(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const void *key, const void *value);
int DictRemove(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const size_t index);
int DictIterate(ExistsListNum *existsList, const size_t length, const size_t startIndex, size_t *nextElementIndexDest);
int DictResize(ExistsListNum **existsList, void **body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const size_t newLength);
size_t DictDefaultHash(size_t keySize, void *key);
int DictDefaultEquate(size_t keySize, void *keyA, void *keyB);

#endif
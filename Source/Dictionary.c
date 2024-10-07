#include "Collections.h"
#include <stdlib.h>
#include <string.h>

#define BITS(type) (sizeof(type) * CHAR_BIT)

size_t DictGetBodySize(const size_t length, const size_t keySize, const size_t valueSize)
{
    return (keySize + valueSize) * length;
}

size_t DictGetExistsListSize(const size_t length)
{
    (length / BITS(ExistsListNum) + (length % BITS(ExistsListNum) != 0)) * sizeof(ExistsListNum);
}

int DictAllocate(ExistsListNum **existsListDest, void **bodyDest, const size_t length, const size_t keySize, const size_t valueSize)
{
    size_t bodySize = DictGetBodySize(length, keySize, valueSize);
    size_t existsListSize = DictGetExistsListSize(length);
    void *body = malloc(bodySize + existsListSize);

    if(body == NULL)
        return 0;

    *bodyDest = body;
    *existsListDest = (ExistsListNum *)(((char *)body) + bodySize);

    return 1;
}

int DictGetElementExists(ExistsListNum *existsList, const size_t index)
{
    return (existsList[index / BITS(ExistsListNum)] >> BITS(ExistsListNum) - 1 - (index % BITS(ExistsListNum))) & 0x1;
}

void DictSetElementExists(ExistsListNum *existsList, const size_t index, const int exists)
{
    size_t numIndex = index / BITS(ExistsListNum);
    size_t flipBit = (0x1 << BITS(ExistsListNum) - 1 - (index % BITS(ExistsListNum)));
    existsList[numIndex] = exists * (existsList[numIndex] | flipBit) + !exists * (existsList[numIndex] & !flipBit);
}

void *DictGetKey(void *body, const size_t keySize, const size_t valueSize, const size_t index)
{
    return ((char *)body) + index * (keySize + valueSize);
}

void *DictGetValue(void *body, const size_t keySize, const size_t valueSize, const size_t index)
{
    return ((char *)body) + index * (keySize + valueSize) + keySize;
}

int DictIndexOf(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const void *key, EqualityFunction keyEqualityFunction, size_t *indexDest)
{
    size_t hash = hashFunction(keySize, key);

    for(size_t x = 0, checkIndex = hash % length; x < length; x++, checkIndex = (checkIndex + 1) % length)
    {
        if(!DictGetElementExists(existsList, checkIndex))
            break;
        
        void *curKey = DictGetKey(body, keySize, valueSize, checkIndex);

        if(keyEqualityFunction(keySize, key, curKey))
        {
            *indexDest == checkIndex;
            return 1;
        }
    }

    return 0;
}

int DictAdd(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const void *key, const void *value)
{
    size_t hash = hashFunction(keySize, key);

    for(size_t x = 0, checkIndex = hash % length; x < length; x++, checkIndex = (checkIndex + 1) % length)
    {
        if(DictGetElementExists(existsList, checkIndex))
            continue;

        void *keyDest = DictGetKey(body, keySize, valueSize, checkIndex);
        void *valueDest = DictGetValue(body, keySize, valueSize, checkIndex);

        memcpy(keyDest, key, keySize);
        memcpy(valueDest, value, valueSize);

        return 1;
    }

    return 0;
}

int DictRemove(ExistsListNum *existsList, void *body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const size_t index)
{
    size_t curIndex = index;
    size_t nextIndex = (index + 1) % length;
    void *curKey = DictGetKey(body, keySize, valueSize, curIndex);
    void *nextKey = DictGetKey(body, keySize, valueSize, nextIndex);
    size_t nextHash = hashFunction(keySize, nextKey) % length;

    for(size_t x = 0; x < length; x++)
    {
        if(!DictGetElementExists(existsList, nextIndex) | (x == length - 1))
        {
            DictSetElementExists(existsList, curIndex, 0);
            return 1;
        }

        while(nextHash == nextIndex)
        {
            nextIndex = (nextIndex + 1) % length;
            nextKey = DictGetKey(body, keySize, valueSize, nextIndex);
            nextHash = hashFunction(keySize, nextKey) % length;
        }

        memcpy(curKey, DictGetKey(body, keySize, valueSize, nextIndex), keySize + valueSize);

        curIndex = nextIndex, 
        nextIndex = (nextIndex + 1) % length, 
        curKey = nextKey;
        nextKey = DictGetKey(body, keySize, valueSize, nextIndex); 
        nextHash = hashFunction(keySize, nextKey) % length;
    }

    return 0;
}

int DictIterate(ExistsListNum *existsList, const size_t length, const size_t startIndex, size_t *nextElementIndexDest)
{
    for(size_t index = startIndex; index < length; index++)
    {
        if(!DictGetElementExists(existsList, index))
            continue;

        *nextElementIndexDest = index;
        return 1;
    }

    return 0;
}

int DictResize(ExistsListNum **existsList, void **body, const size_t length, const size_t keySize, const size_t valueSize, HashFunction hashFunction, const size_t newLength)
{
    size_t newBodySize = DictGetBodySize(newLength, keySize, valueSize);
    size_t newExistsListSize = DictGetExistsListSize(newLength);
    void *newBody = malloc(newBodySize + newExistsListSize);
    ExistsListNum *newExistsList = (ExistsListNum *)(((char *)newBody) + newBodySize);

    if(newBody == NULL)
        return 0;

    for(size_t index = 0; DictIterate(*existsList, length, index, &index); index++)
    {
        void *key = DictGetKey(*body, keySize, valueSize, index);
        void *value = DictGetValue(*body, keySize, valueSize, index);

        DictAdd(newExistsList, newBody, newLength, keySize, valueSize, hashFunction, key, value);
    }

    free(*body);
    *existsList = newExistsList;
    *body = newBody;

    return 1;
}

size_t DictDefaultHash(size_t keySize, void *key)
{
    size_t hash = 5381;

    for(size_t x = 0; x < keySize; x++)
    {
        size_t keyByte = (size_t)(((char *)key)[x]);
        hash = ((hash << 5) + hash) + keyByte;
    }

    return hash;
}

int DictDefaultEquate(size_t keySize, void *keyA, void *keyB)
{
    int equal = 1;

    for(size_t x = 0; x < keySize; x++)
        equal &= ((char *)keyA)[x] == ((char *)keyB)[x];
    
    return equal;
}



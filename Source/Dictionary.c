#include "Collections.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define SIZE_BITS(type) (sizeof(type) * CHAR_BIT)
typedef size_t ExistsListInt;

static inline size_t DictGetExistsListCount(const size_t length)
{
    return length / SIZE_BITS(ExistsListNum) + (length % SIZE_BITS(ExistsListNum) != 0);
}

static inline ExistsListInt *DictGetExistsList(const Dictionary *dictionary, const size_t keySize, const size_t valueSize)
{
    return (ExistsListInt *)((char *)dictionary->Body + (keySize + valueSize) * dictionary->Length);
}

static inline int DictGetElementExists(const Dictionary *dictionary, const size_t keySize, const size_t valueSize, const size_t index)
{
    ExistsListInt *existsList = DictGetExistsList(dictionary, keySize, valueSize);
    return (existsList[index / SIZE_BITS(ExistsListNum)] >> SIZE_BITS(ExistsListNum) - 1 - (index % SIZE_BITS(ExistsListNum))) & 0x1;
}

static inline void DictSetElementExists(const Dictionary *dictionary, const size_t keySize, const size_t valueSize, const size_t index, const int exists)
{
    ExistsListInt *existsList = DictGetExistsList(dictionary, keySize, valueSize);

    size_t numIndex = index / SIZE_BITS(ExistsListNum);
    size_t flipBit = 0x1 << (SIZE_BITS(ExistsListNum) - 1 - (index % SIZE_BITS(ExistsListNum)));

    existsList[numIndex] = exists * (existsList[numIndex] | flipBit) + !exists * (existsList[numIndex] & ~flipBit);
}

size_t DictGetSize(const size_t length, const size_t keySize, const size_t valueSize)
{
    return length * (keySize + valueSize) + DictGetExistsListCount(length) * sizeof(ExistsListInt);
}

void DictInit(Dictionary *dictionary, const size_t keySize, const size_t valueSize)
{
    ExistsListInt *existsList = DictGetExistsList(dictionary, keySize, valueSize);
    size_t existsListCount = DictGetExistsListCount(dictionary->Length);

    for(size_t x = 0; x < existsListCount; x++)
        existsList[x] = 0;
}

int DictIndexOf(const Dictionary *dictionary, const size_t keySize, const size_t valueSize, const HashFunction hashFunction, const EqualityFunction keyEqualityFunction, const void *key, size_t *indexDest)
{
    size_t hash = hashFunction(keySize, key);

    for(size_t x = 0, checkIndex = hash % dictionary->Length; x < dictionary->Length; x++, checkIndex = (checkIndex + 1) % dictionary->Length)
    {
        if(!DictGetElementExists(dictionary, keySize, valueSize, checkIndex))
            break;
        
        void *curKey = DictGetKey(dictionary, keySize, valueSize, checkIndex);

        if(keyEqualityFunction(keySize, key, curKey))
            continue;

        *indexDest = checkIndex;
        return 0;
    }

    return 1;
}

int DictResize(Dictionary *dictionary, const size_t keySize, const size_t valueSize, const HashFunction hashFunction, const size_t newLength)
{
    Dictionary newDictionary;
    newDictionary.Count = 0;
    newDictionary.Length = newLength;
    newDictionary.Body = malloc(DictGetSize(newLength, keySize, valueSize));

    if(newDictionary.Body == NULL)
        return 1;

    DictInit(&newDictionary, keySize, valueSize);
    int result = 0;

    for(size_t index = 0; !DictIterate(dictionary, keySize, valueSize, &index) && !result; index++)
    {   
        void *key = DictGetKey(dictionary, keySize, valueSize, index);
        void *value = DictGetValue(dictionary, keySize, valueSize, index);

        result = DictAdd(&newDictionary, keySize, valueSize, hashFunction, 90, key, value);
    }

    if(result)
    {
        free(newDictionary.Body);
        return result;
    }

    free(dictionary->Body);
    *dictionary = newDictionary;

    return 0;
}

int DictAdd(Dictionary *dictionary, const size_t keySize, const size_t valueSize, const HashFunction hashFunction, const size_t resizePercentage, const void *key, const void *value)
{
    if(dictionary->Length == 0 || dictionary->Count * 100 / dictionary->Length >= resizePercentage)
    {
        int result;
        result = DictResize(dictionary, keySize, valueSize, hashFunction, dictionary->Length * 2 + (dictionary->Length == 0));

        if(result)
            return result;
    }

    size_t hash = hashFunction(keySize, key);

    for(size_t x = 0, checkIndex = hash % dictionary->Length; x < dictionary->Length; x++, checkIndex = (checkIndex + 1) % dictionary->Length)
    {
        if(DictGetElementExists(dictionary, keySize, valueSize, checkIndex))
            continue;
            
        void *keyDest = DictGetKey(dictionary, keySize, valueSize, checkIndex);
        void *valueDest = DictGetValue(dictionary, keySize, valueSize, checkIndex);

        memcpy(keyDest, key, keySize);
        memcpy(valueDest, value, valueSize);

        DictSetElementExists(dictionary, keySize, valueSize, checkIndex, 1);
        dictionary->Count += 1;
        return 0;
    }

    return 1;
}

void DictRemove(Dictionary *dictionary, const size_t keySize, const size_t valueSize, const HashFunction hashFunction, const size_t index)
{
    size_t curIndex = index;
    size_t nextIndex = index + 1;
    void *curKey = DictGetKey(dictionary, keySize, valueSize, curIndex);
    void *nextKey = DictGetKey(dictionary, keySize, valueSize, nextIndex % dictionary->Length);
    size_t nextHash = hashFunction(keySize, nextKey) % dictionary->Length;

    for(size_t x = 0; x < dictionary->Length; x++)
    {
        if(!DictGetElementExists(dictionary, keySize, valueSize, nextIndex % dictionary->Length))
            break;

        if(nextHash <= curIndex)
        {
            memcpy(curKey, DictGetKey(dictionary, keySize, valueSize, nextIndex % dictionary->Length), keySize + valueSize);

            curIndex = nextIndex;
            curKey = nextKey;
        }

        nextIndex++;
        nextKey = DictGetKey(dictionary, keySize, valueSize, nextIndex % dictionary->Length); 
        nextHash = hashFunction(keySize, nextKey) % dictionary->Length;
    }

    DictSetElementExists(dictionary, keySize, valueSize, curIndex % dictionary->Length, 0);
    dictionary->Count -= 1;
}

int DictIterate(const Dictionary *dictionary, const size_t keySize, const size_t valueSize, size_t *index)
{
    for(; *index < dictionary->Length; *index += 1)
    {
        if(!DictGetElementExists(dictionary, keySize, valueSize, *index))
            continue;

        return 0;
    }

    return 1;
}

size_t DictDefaultHash(const size_t keySize, const void *key)
{
    size_t hash = 5381;

    for(size_t x = 0; x < keySize; x++)
    {
        size_t keyByte = (size_t)(((char *)key)[x]);
        hash = ((hash << 5) + hash) + keyByte;
    }

    return hash;
}

int DictDefaultEquate(const size_t keySize, const void *keyA, const void *keyB)
{
    int equal = 0;

    for(size_t x = 0; x < keySize; x++)
        equal |= ((char *)keyA)[x] != ((char *)keyB)[x];
    
    return equal;
}
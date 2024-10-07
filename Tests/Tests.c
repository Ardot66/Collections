#include <stdio.h>
#include <string.h>
#include "../Source/Collections.h"

#define PASS(message) {printf("Test Passed: %s\n", message); TestCount ++; TestsPassed ++;}
#define FAIL(message) {printf("Test Failed: %s\n", message); TestCount ++;}

size_t TestCount = 0;
size_t TestsPassed = 0;

void TestCArray()
{
    const size_t startingCArrayLength = 10;
    size_t cArrayLength = startingCArrayLength;
    size_t testData[cArrayLength];

    size_t cArrayOffset = 0;
    size_t cArrayCount = 0;
    size_t *cArray = malloc(sizeof(size_t) * cArrayLength);

    for(size_t x = 0; x < cArrayLength; x++)
    {
        testData[x] = (x * 129 / 3) % 61;
        CArraySet(cArray, cArrayLength, cArrayOffset, sizeof(size_t), cArrayCount, testData + x);
        cArrayCount++;
    }

    CArrayRemove(cArray, &cArrayCount, cArrayLength, &cArrayOffset, sizeof(size_t), 0);
    CArrayRemove(cArray, &cArrayCount, cArrayLength, &cArrayOffset, sizeof(size_t), 0);
    size_t value = 0;
    CArrayGet(cArray, cArrayLength, cArrayOffset, sizeof(size_t), 1, &value);
    CArrayInsert(cArray, &cArrayCount, cArrayLength, &cArrayOffset, sizeof(size_t), 1, &value);
    CArrayRemove(cArray, &cArrayCount, cArrayLength, &cArrayOffset, sizeof(size_t), 2);
    CArrayRemove(cArray, &cArrayCount, cArrayLength, &cArrayOffset, sizeof(size_t), cArrayCount);

    if(cArrayCount == cArrayLength - 3 && cArrayOffset == 2)
        PASS("Successfully removed items from CArray")
    else
        FAIL("Failed to remove items from CArray")

    if(!CArrayResize((void**)&cArray, cArrayCount, &cArrayLength, &cArrayOffset, sizeof(size_t), cArrayLength - 3) || cArrayCount != cArrayLength || !CArrayResize((void**)&cArray, cArrayCount, &cArrayLength, &cArrayOffset, sizeof(size_t), cArrayLength + 50))
        FAIL("Failed to resize CArray")
    else
        PASS("Successfully resized CArray")

    int getSuccessful = 1;

    for(size_t x = 0, index = 0; x < startingCArrayLength; x++, index++)
    {
        size_t value;

        if(x == 0 || x == 1 || x == startingCArrayLength - 1)
        {
            index --;
            continue;
        }

        CArrayGet(cArray, cArrayLength, cArrayOffset, sizeof(size_t), index, &value);
        getSuccessful &= value == testData[x];
    }

    if(getSuccessful)
        PASS("Successfully read data from CArray\n")
    else
        FAIL("Failed to read data from CArray\n")

    free(cArray);
}

void TestArray()
{
    const size_t arrayInitialLength = 10;
    size_t arrayLength = arrayInitialLength;
    size_t testData[arrayLength];

    size_t arrayOffset = 0;
    size_t arrayCount = 0;
    size_t *array = malloc(sizeof(size_t) * arrayLength);

    for(ssize_t x = arrayInitialLength - 1; x >= 0; x--)
    {
        testData[x] = (x * 129 / 3) % 61;
        ArrayInsert(array, &arrayCount, sizeof(size_t), 0, testData + x);
    }

    ArrayRemove(array, &arrayCount, sizeof(size_t), 0);
    ArrayRemove(array, &arrayCount, sizeof(size_t), arrayCount - 1);

    if(!ArrayResize((void**)&array, &arrayLength, sizeof(size_t), arrayLength - 2) || arrayLength != arrayInitialLength - 2 || !ArrayResize((void**)&array, &arrayLength, sizeof(size_t), arrayInitialLength + 20) || arrayLength != arrayInitialLength + 20)
        FAIL("Failed to resize Array")
    else
        PASS("Successfully resized Array")

    int getSuccessful = 1;

    for(size_t x = 0, index = 0; x < arrayInitialLength; x++)
    {
        if(x == 0 || x == arrayInitialLength - 1)
            continue;
        
        size_t value = array[index];
        getSuccessful &= value == testData[x];

        index++;
    }

    if(getSuccessful)
        PASS("Successfully retrieved values from Array")
    else
        FAIL("Failed to retrieve values from Array")

    free(array);
}

int DictGetElementExists(ExistsListNum *existsList, const size_t index);

void TestDictionary()
{
    struct KeyValuePair
    {
        size_t Key;
        size_t Value;
    };

    const size_t dictInitialLength = 10;
    size_t dictLength = dictInitialLength;
    void *body;
    ExistsListNum *existsList;

    int allocationSuccess = DictAllocate(&existsList, &body, dictLength, sizeof(size_t), sizeof(size_t));

    size_t dictValuesLength = dictInitialLength;
    struct KeyValuePair dictValues[dictValuesLength];

    int addSuccessful = 1;

    for(size_t x = 0; x < dictValuesLength; x++)
    {
        struct KeyValuePair dictValue;
        dictValue.Key = (x * 129 / 3);
        dictValue.Value = x;

        dictValues[x] = dictValue;

        addSuccessful &= DictAdd(existsList, body, dictLength, sizeof(size_t), sizeof(size_t), DictDefaultHash, &dictValue.Key, &dictValue.Value);
    }

    if(addSuccessful)
        PASS("Successfully added items to dictionary")
    else
        FAIL("Failed to add items to dictionary")

    size_t index = 0;

    for(size_t x = 0; x < dictInitialLength; x++)
    {
        printf("%llu, Key: %llu Value: %llu Exists: %d Hash: %llu \n", x, *(size_t *)DictGetKey(body, sizeof(size_t), sizeof(size_t), x), *(size_t *)DictGetValue(body, sizeof(size_t), sizeof(size_t), x), DictGetElementExists(existsList, x), DictDefaultHash(sizeof(size_t), DictGetKey(body, sizeof(size_t), sizeof(size_t), x)) % dictLength);
    }

    DictIndexOf(existsList, body, dictLength, sizeof(size_t), sizeof(size_t), DictDefaultHash, &(dictValues[3].Key), DictDefaultEquate, &index);
    DictRemove(existsList, body, dictLength, sizeof(size_t), sizeof(size_t), DictDefaultHash, index);

    for(size_t x = 0; x < dictInitialLength; x++)
    {
        printf("%llu, Key: %llu Value: %llu Exists: %d Hash: %llu \n", x, *(size_t *)DictGetKey(body, sizeof(size_t), sizeof(size_t), x), *(size_t *)DictGetValue(body, sizeof(size_t), sizeof(size_t), x), DictGetElementExists(existsList, x), DictDefaultHash(sizeof(size_t), DictGetKey(body, sizeof(size_t), sizeof(size_t), x)) % dictLength);
    }

    int resizeSuccessful = DictResize(&existsList, &body, &dictLength, sizeof(size_t), sizeof(size_t), DictDefaultHash, dictLength - 1);

    for(size_t x = 0; x < dictInitialLength; x++)
    {
        printf("%llu, Key: %llu Value: %llu Exists: %d Hash: %llu \n", x, *(size_t *)DictGetKey(body, sizeof(size_t), sizeof(size_t), x), *(size_t *)DictGetValue(body, sizeof(size_t), sizeof(size_t), x), DictGetElementExists(existsList, x), DictDefaultHash(sizeof(size_t), DictGetKey(body, sizeof(size_t), sizeof(size_t), x)) % dictLength);
    }

    //printf

    int getSuccessful = 1;

    for(size_t x = 0; x < dictValuesLength; x++)
    {
        struct KeyValuePair dictValue = dictValues[x];

        if(dictValue.Value == 3)
            continue;

        size_t valueIndex = 0;
        int indexOfSuccessful = DictIndexOf(existsList, body, dictLength, sizeof(size_t), sizeof(size_t), DictDefaultHash, &dictValue.Key, DictDefaultEquate, &valueIndex);
        getSuccessful &= indexOfSuccessful;

        if(!indexOfSuccessful)
            printf("Unsuccessful, Key: %llu\n", dictValue.Key);

        size_t *value = (size_t *)DictGetValue(body, sizeof(size_t), sizeof(size_t), valueIndex);

        getSuccessful &= dictValue.Value == *value;
    }

    if(getSuccessful)
        PASS("Successfully retrieved items from dictionary")
    else
        FAIL("Failed to retrieve items from dictionary")

    free(body);
}

int main (int argCount, char **argValues)
{
    TestCArray();
    TestArray();
    TestDictionary();

    printf("%llu out of %llu tests passed\n", TestsPassed, TestCount);

    return 0;
}
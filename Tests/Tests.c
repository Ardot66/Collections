#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Collections.h"
#include "TestingUtilities.h"

#define PASS(message) {printf("Test Passed: %s\n", message); TestCount ++; TestsPassed ++;}
#define FAIL(message) {printf("Test Failed: %s\n", message); TestCount ++;}

size_t TestCount = 0;

void TestCArray()
{
    printf("\nTesting CArray\n-----\n");
    //Internal array type is size_t
    CArray cArray;

    cArray.Count = 1;
    cArray.Length = 1;
    cArray.Offset = 0;
    cArray.Body = malloc(sizeof(size_t));

    TEST(cArray.Body, !=, NULL, p, return;)
    TEST(CArrayGet(&cArray, sizeof(size_t), 0), ==, cArray.Body, p, return;)

    const size_t preInsertCheckNum = 1;
    *(size_t *)CArrayGet(&cArray, sizeof(size_t), 0) = preInsertCheckNum;

    int result = 0;
    const size_t insertedAmount = 2;
    result = CArrayInsert(&cArray, sizeof(size_t), 1, &insertedAmount);
    
    TEST(result, ==, 0, llu, return;)
    TEST(cArray.Count, ==, 2, llu)
    TEST(cArray.Length, >=, cArray.Count, llu)

    const size_t insertedAmount2 = 3;
    TEST(result = CArrayInsert(&cArray, sizeof(size_t), 0, &insertedAmount2), ==, 0, llu, return;)
    TEST(*(size_t *)CArrayGet(&cArray, sizeof(size_t), 1), ==, preInsertCheckNum, llu)
    
    CArrayRemove(&cArray, sizeof(size_t), 1);
    const size_t postRemoveCheckNum = 4;
    TEST(result = CArrayInsert(&cArray, sizeof(size_t), 1, &postRemoveCheckNum), ==, 0, llu, return;)

    TEST(result = CArrayResize(&cArray, sizeof(size_t), cArray.Length * 2), ==, 0, llu, return;)

    TEST(*(size_t *)CArrayGet(&cArray, sizeof(size_t), 0), ==, insertedAmount2, llu)
    TEST(*(size_t *)CArrayGet(&cArray, sizeof(size_t), 1), ==, postRemoveCheckNum, llu)
    TEST(*(size_t *)CArrayGet(&cArray, sizeof(size_t), 2), ==, insertedAmount, llu)

    free(cArray.Body);
}

void TestArray()
{
    printf("\nTesting Array\n-----\n");

    size_t length = 1;
    size_t count = 1;
    size_t *array = malloc(sizeof(size_t));

    TEST(array, !=, NULL, p, return;)

    const size_t testNum1 = 1;
    array[0] = testNum1;

    const size_t testNum2 = 2;
    int result;
    TEST(result = ArrayInsert((void **)&array, &count, &length, sizeof(size_t), 0, &testNum2), ==, 0, llu, return;);

    TEST(count, ==, 2, llu)
    TEST(length, >=, count, llu)
    TEST(array[0], ==, testNum2, llu)
    TEST(array[1], ==, testNum1, llu)

    ArrayRemove(array, &count, sizeof(*array), 0);

    TEST(array[0], ==, testNum1, llu)

    free(array);
}

int DictGetElementExists(ExistsListNum *existsList, const size_t index);

void TestDictionary()
{
    printf("\nTesting Dictionary\n-----\n");

    struct KeyValuePair
    {
        size_t Key;
        size_t Value;
    };

    const size_t dictInitialLength = 1000;
    size_t dictLength = dictInitialLength;
    void *body;
    ExistsListNum *existsList;

    int allocationSuccess = DictAllocate(&existsList, &body, dictLength, sizeof(size_t), sizeof(size_t));

    size_t dictValuesLength = dictInitialLength / 2;
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

    DictIndexOf(existsList, body, dictLength, sizeof(size_t), sizeof(size_t), DictDefaultHash, &(dictValues[3].Key), DictDefaultEquate, &index);
    DictRemove(existsList, body, dictLength, sizeof(size_t), sizeof(size_t), DictDefaultHash, index);

    int resizeSuccessful = DictResize(&existsList, &body, &dictLength, sizeof(size_t), sizeof(size_t), DictDefaultHash, dictLength - 1);

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

    DictFree(body);
}

int main (int argCount, char **argValues)
{
    TestCArray();
    TestArray();
    TestDictionary();

    TestsEnd();
    return 0;
}
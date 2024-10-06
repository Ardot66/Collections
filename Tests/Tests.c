#include <stdio.h>
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
}

int main (int argCount, char **argValues)
{
    TestCArray();

    printf("%llu out of %llu tests passed\n", TestsPassed, TestCount);
}
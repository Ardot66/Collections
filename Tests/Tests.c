#include <stdio.h>
#include "../Source/Collections.h"

#define PASS(message) {printf("Test Passed: %s\n", message); TestCount ++; TestsPassed ++;}
#define FAIL(message) {printf("Test Failed: %s\n", message); TestCount ++;}

size_t TestCount = 0;
size_t TestsPassed = 0;

void TestList()
{
    List list;
    
    if(!ListCreate(sizeof(size_t), 10, &list))
        FAIL("Failed to create list")
    else
        PASS("Successfully created list")

    size_t testDataCount = 10;
    size_t testData[testDataCount];

    int addSuccessful = 1;

    for(size_t x = 0; x < testDataCount; x++)
    {
        testData[x] = (x * 129 / 3) % 61;
        addSuccessful &= ListAdd(&list, testData + x);
    }

    if(addSuccessful)
        PASS("Successfully added elements to list")
    else
        FAIL("Failed to add elements to list")

    size_t endValue;
    ListPopEnd(&list, &endValue);

    size_t frontValue;
    ListPopFront(&list, &frontValue);
    ListRemove(&list, 1);

    if(list.Count == testDataCount - 3 && list.Offset == 1 && frontValue == testData[0] && endValue == (size_t)(testData[testDataCount - 1]))
        PASS("Successfully removed items from list")
    else
        FAIL("Failed to remove items from list")

    if(!ListResize(&list, testDataCount - 3) || list.Length != testDataCount - 3)
        FAIL("Failed to resize list")
    else
        PASS("Successfully resized list")

    int getSuccessful = 1;

    for(size_t x = 0, index = 0; x < testDataCount; x++, index++)
    {
        size_t value;

        if(x == 0 || x == 2 || x == testDataCount - 1)
        {
            index --;
            continue;
        }

        ListGet(&list, index, &value);
        getSuccessful &= value == testData[x];
    }

    if(getSuccessful)
        PASS("Successfully read data from list\n")
    else
        FAIL("Failed to read data from list\n")
}

int main (int argCount, char **argValues)
{
    TestList();

    printf("%llu out of %llu tests passed\n", TestsPassed, TestCount);
}
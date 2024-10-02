#include <stdio.h>
#include "../Source/Collections.h"

#define PASS(message) {printf("Test Passed: %s\n", message); testCount ++; testsPassed ++;}
#define FAIL(message) {printf("Test Failed: %s\n", message); testCount ++;}

int main (int argCount, char **argValues)
{
    size_t testCount = 0;
    size_t testsPassed = 0;    

    List list;
    
    if(!ListCreate(sizeof(size_t), 10, &list))
        FAIL("Failed to create list")
    else
        PASS("Successfully created list")

    if(!ListResize(&list, 100) || list.Length != 100)
        FAIL("Failed to resize list")
    else
        PASS("Successfully resized list")

    size_t testDataCount = 10;
    size_t testData[testDataCount];

    int addSuccessful = 1;

    for(size_t x = 0; x < testDataCount; x++)
    {
        testData[x] = (x * 129 / 3) % 7;
        ListAdd(&list, testData + x);
    }

    for(size_t x = 0; x < testDataCount; x++)
    {
        size_t value;

        ListGet(&list, x, &value);
        addSuccessful &= value == testData[x];
    }

    if(addSuccessful)
        PASS("Successfully added and read data from list\n")
    else
        FAIL("Failed to add and read data from list\n")

    printf("%llu out of %llu tests passed\n", testsPassed, testCount);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Collections.h"
#include "TestingUtilities.h"

void TestCArray()
{
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


#define SIZES sizeof(size_t), sizeof(size_t)
void TestDictionaryValue(const Dictionary *dictionary, const size_t key, const size_t checkValue)
{
    size_t valueIndex;
    TEST(DictIndexOf(dictionary, SIZES, DictDefaultHash, DictDefaultEquate, &key, &valueIndex), ==, 0, d, return;)
    TEST(*(size_t *)DictGetKey(dictionary, SIZES, valueIndex), ==, checkValue, llu);
}

void TestDictionary()
{
    typedef struct KeyValuePair
    {
        size_t Key;
        size_t Value;
    } KeyValuePair;

    Dictionary dictionary;
    dictionary.Length = 4;
    dictionary.Count = 0;
    dictionary.Body = malloc(DictGetSize(dictionary.Length, SIZES));

    TEST(dictionary.Body, !=, NULL, p, return;)
    DictInit(&dictionary, SIZES);

    const KeyValuePair pair1 = {.Key = 1, .Value = 1};
    TEST(DictAdd(&dictionary, SIZES, DictDefaultHash, 60, &pair1.Key, &pair1.Value), ==, 0, llu)

    TestDictionaryValue(&dictionary, pair1.Key, pair1.Value);

    const size_t fillerStart = 256;
    const size_t fillerAmount = 50;
    int addFillerSuccessful = 1;

    for(size_t x = fillerStart; x < fillerAmount + fillerStart; x++)
    {
        KeyValuePair fillerPair = {.Key = x, .Value = x};
        addFillerSuccessful &= !DictAdd(&dictionary, SIZES, DictDefaultHash, 60, &fillerPair.Key, &fillerPair.Value);

        if(!addFillerSuccessful)
            break;
    }

    TEST(addFillerSuccessful, ==, 1, d)

    TestDictionaryValue(&dictionary, pair1.Key, pair1.Value);

    const size_t fillerRemoveAmount = 10;
    int removeFillerSuccessful = 1;

    for(size_t x = fillerStart; x < fillerRemoveAmount + fillerStart; x++)
    {
        size_t index;
        removeFillerSuccessful &= !DictIndexOf(&dictionary, SIZES, DictDefaultHash, DictDefaultEquate, &x, &index);

        if(!removeFillerSuccessful)
            break;

        DictRemove(&dictionary, SIZES, DictDefaultHash, index);
    }

    TEST(removeFillerSuccessful, ==, 1, d)

    TestDictionaryValue(&dictionary, pair1.Key, pair1.Value);
}
#undef SIZES

int main (int argCount, char **argValues)
{
    TestCArray();
    TestArray();
    TestDictionary();

    printf("\n");
    TestsEnd();
    printf("-----\n");
    return 0;
}
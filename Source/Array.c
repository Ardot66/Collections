#include "Collections.h"
#include <stdlib.h>
#include <string.h>

int ArrayResize(void **array, size_t *length, const size_t elementSize, const size_t newLength)
{
    void *temp = realloc(*array, elementSize * newLength);   

    if(temp == NULL)
        return errno;

    *array = temp;
    *length = newLength;

    return 0;
}

int ArrayInsert(void **array, size_t *count, size_t *length, const size_t elementSize, const size_t index, const void *element)
{
    if(*count >= *length)
    {
        size_t newLength = *length * 2 + (*length == 0);

        int result = ArrayResize(array, length, elementSize, newLength);
        if(result)
            return result;
    }

    char *const arrayBody = (char *)*array;

    for(size_t x = *count * elementSize; x > index * elementSize; x--)
        arrayBody[x] = arrayBody[x - elementSize];

    memcpy(arrayBody + index * elementSize, element, elementSize);

    *count += 1;
}

void ArrayRemove(void *array, size_t *count, const size_t elementSize, const size_t index)
{
    char *const arrayBody = (char *)array; 
    *count -= 1;

    for(size_t x = index * elementSize; x < *count * elementSize; x++)
        arrayBody[x] = arrayBody[x + elementSize];
}
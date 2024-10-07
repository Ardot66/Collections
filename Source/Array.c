#include "Collections.h"
#include <stdlib.h>
#include <string.h>

int ArrayResize(void **array, size_t *length, const size_t elementSize, const size_t newLength)
{
    void *temp = realloc(*array, elementSize * newLength);
    
    if(temp == NULL)
        return 0;

    *array = temp;
    *length = newLength;

    return 1;
}

void ArrayInsert(void *array, size_t *count, const size_t elementSize, const size_t index, const void *element)
{
    for(size_t x = *count * elementSize; x > index * elementSize; x--)
        ((char *)array)[x] = ((char *)array)[x - elementSize];

    memcpy(((char *)array) + index * elementSize, element, elementSize);

    *count += 1;
}

void ArrayRemove(void *array, size_t *count, const size_t elementSize, const size_t index)
{
    *count -= 1;

    for(size_t x = index * elementSize; x < *count * elementSize; x++)
        ((char *)array)[x] = ((char*)array)[x + elementSize];
}
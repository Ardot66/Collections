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
    if(index > *count)
        *count = index + 1;

    ARRAY_GENERIC_TRY_RESIZE(*count, *length,
        ArrayResize(array, length, elementSize, *count * 2 + (*length == 0)), 
        return result;
    )

    char *arrayBody = (char *)*array;
    ARRAY_GENERIC_RIGHTSHIFT(char, elementSize, 1, index, *count - index, arrayBody + getIndex)
    ARRAY_GENERIC_SET(char, elementSize, index, element, arrayBody + getIndex)
    *count += 1;

    return 0;
}

void ArrayRemove(void *array, size_t *count, const size_t elementSize, const size_t index)
{
    *count -= 1;
    char *arrayBody = (char *)array;
    ARRAY_GENERIC_LEFTSHIFT(char, elementSize, 1, index + 1, *count - index, arrayBody + getIndex)
}
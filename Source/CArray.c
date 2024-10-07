#include "Collections.h"
#include <stdlib.h>
#include <string.h>

void *CArrayGet(const void *array, const size_t length, const size_t offset, const size_t elementSize, const size_t index)
{
    return ((char *)array) + ((index + offset) % length) * elementSize;
}

void CArrayResizeElements(void *array, const size_t count, const size_t length, size_t *arrayOffset, const size_t elementSize, const size_t newLength)
{
    size_t newOffset = (((*arrayOffset + count) % length) % newLength);
    char *arrayBody = ((char *)array);

    for(size_t x = 0; x < count * elementSize; x++)
        arrayBody[(x + newOffset * elementSize) % (newLength * elementSize)] = arrayBody[(x + *arrayOffset * elementSize) % (length * elementSize)];

    *arrayOffset = newOffset;
}

int CArrayResize(void **array, const size_t count, size_t *length, size_t *arrayOffset, const size_t elementSize, const size_t newLength)
{
    if(count > newLength)
        return 0;

    if(*length >= newLength)
        CArrayResizeElements(*array, count, *length, arrayOffset, elementSize, newLength);

    void *tempBody = realloc(*array, newLength * elementSize);

    if(tempBody == NULL)
        return 0;
    
    *array = tempBody;

    if(*length < newLength)
        CArrayResizeElements(*array, count, *length, arrayOffset, elementSize, newLength);

    *length = newLength;
    return 1;
}

void CArrayInsert(void *array, size_t *count, const size_t length, size_t *offset, const size_t elementSize, const size_t index, const void *element)
{
    if(index == 0)
        *offset = (*offset <= 0) * (length - 1) + (*offset > 0) * ((*offset - 1) % length);
    else
        for(size_t x = *count; x > index; x--)
            memcpy(CArrayGet(array, length, *offset, elementSize, x), CArrayGet(array, length, *offset, elementSize, x - 1), elementSize);

    *count += 1;
    memcpy(CArrayGet(array, length, *offset, elementSize, index), element, elementSize);
}

void CArrayRemove(void *array, size_t *count, const size_t length, size_t *offset, const size_t elementSize, const size_t index)
{
    *count -= 1;

    if(index == 0)
    {
        *offset = (*offset + 1) % length;
        return;
    }

    for(size_t x = index; x < *count; x++)
        memcpy(CArrayGet(array, length, *offset, elementSize, x), CArrayGet(array, length, *offset, elementSize, x + 1), elementSize);
}
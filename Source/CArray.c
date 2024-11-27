#include "Collections.h"
#include <stdlib.h>
#include <string.h>

#define CARRAY_GETTER (char *)cArray->Body + (getIndex + cArray->Offset * elementSize) % (cArray->Length * elementSize)

static void CArrayResizeElements(CArray *cArray, const size_t elementSize, const size_t newLength)
{
    size_t newOffset = (((cArray->Offset + cArray->Count) % cArray->Length) % newLength);
    char *arrayBody = ((char *)cArray->Body);

    for(size_t x = 0; x < cArray->Count * elementSize; x++)
        arrayBody[(x + newOffset * elementSize) % (newLength * elementSize)] = arrayBody[(x + cArray->Offset * elementSize) % (cArray->Length * elementSize)];

    cArray->Offset = newOffset;
}

int CArrayResize(CArray *cArray, const size_t elementSize, const size_t newLength)
{
    if(cArray->Length == newLength)
        return 0;

    if(cArray->Length >= newLength)
        CArrayResizeElements(cArray, elementSize, newLength);

    void *tempBody = realloc(cArray->Body, newLength * elementSize);

    if(tempBody == NULL)
    {
        if(cArray->Length >= newLength)
        {
            const size_t length = cArray->Length;
            cArray->Length = newLength;

            CArrayResizeElements(cArray, elementSize, length);

            cArray->Length = length;
        }

        return 0;
    }
    
    cArray->Body = tempBody;

    if(cArray->Length > 0 && cArray->Length < newLength)
        CArrayResizeElements(cArray, elementSize, newLength);

    cArray->Length = newLength;
    return 0;
}

int CArrayInsert(CArray *cArray, const size_t elementSize, const size_t index, const void *element)
{
    if(index > cArray->Count)
        cArray->Count = index + 1;

    ARRAY_GENERIC_TRY_RESIZE(cArray->Count, cArray->Length, 
        CArrayResize(cArray, elementSize, cArray->Count * 2 + (cArray->Length == 0)), 
        return result;
    )

    if(index == 0)
        cArray->Offset = (cArray->Offset <= 0) * (cArray->Length - 1) + (cArray->Offset > 0) * ((cArray->Offset - 1) % cArray->Length);
    else
        ARRAY_GENERIC_RIGHTSHIFT(char, elementSize, 1, index, cArray->Count - index, CARRAY_GETTER)

    ARRAY_GENERIC_SET(char, elementSize, index, element, CARRAY_GETTER)
    cArray->Count += 1;

    return 0;
}

void CArrayRemove(CArray *cArray, const size_t elementSize, const size_t index)
{
    cArray->Count -= 1;

    if(index == 0)
        cArray->Offset = (cArray->Offset + 1) % cArray->Length;
    else
        ARRAY_GENERIC_LEFTSHIFT(char, elementSize, 1, index + 1, cArray->Count - index, CARRAY_GETTER)
}
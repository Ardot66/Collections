#include "Collections.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

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

    if(cArray->Length < newLength)
        CArrayResizeElements(cArray, elementSize, newLength);

    cArray->Length = newLength;
    return 0;
}

int CArrayInsert(CArray *cArray, const size_t elementSize, const size_t index, const void *element)
{
    if(cArray->Count >= cArray->Length)
    {
        size_t newLength = cArray->Length * 2 + (cArray->Length == 0);
        int result;

        if(result = CArrayResize(cArray, elementSize, newLength)) return result;
    }

    if(index == 0)
        cArray->Offset = (cArray->Offset <= 0) * (cArray->Length - 1) + (cArray->Offset > 0) * ((cArray->Offset - 1) % cArray->Length);
    else
        for(size_t x = cArray->Count; x > index; x--)
            memcpy(CArrayGet(cArray, elementSize, x), CArrayGet(cArray, elementSize, x - 1), elementSize);

    cArray->Count += 1;
    memcpy(CArrayGet(cArray, elementSize, index), element, elementSize);

    return 0;
}

void CArrayRemove(CArray *cArray, const size_t elementSize, const size_t index)
{
    cArray->Count -= 1;

    if(index == 0)
    {
        cArray->Offset = (cArray->Offset + 1) % cArray->Length;
        return;
    }

    for(size_t x = index; x < cArray->Count; x++)
        memcpy(CArrayGet(cArray, elementSize, x), CArrayGet(cArray, elementSize, x + 1), elementSize);
}
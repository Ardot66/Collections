#ifndef __COLLECTIONS__
#define __COLLECTIONS__

typedef size_t ValidityListNumber;

typedef struct List List;
typedef struct Dictionary Dictionary;
typedef struct DStack DStack;

struct List
{
    void *Body;
    size_t ElementSize;
    size_t Length;
    size_t Count;
    size_t Offset;
};

struct Dictionary
{
    void *ElementsLocation;
    ValidityListNumber *ValidityListLocation;
    size_t ValueSize;
    size_t Length;
    size_t Count;
    
    size_t (*HashFunction)(void *key);
};

struct DStack
{
    void *ElementsLocation;
    ValidityListNumber *ValidityListLocation;
    size_t ElementSize;
    size_t Length;
    size_t Count;
    size_t RemovedCount;
};

#endif
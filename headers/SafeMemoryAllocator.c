#include "SafeMemoryAllocator.h"
#include "malloc.h"

void *allocations[1000];
void **lastAllocation = 0;

void *SMalloc(size_t size)
{
    *lastAllocation = malloc(size);
    ++lastAllocation;
    return *(lastAllocation-1);
}

void *SCalloc(size_t num,size_t size)
{
    *lastAllocation = calloc(num,size);
    ++lastAllocation;
    return *(lastAllocation-1);
}

void FreeAll()
{
    if(!lastAllocation)
        return;
    void **cur = allocations;
    while(cur!=lastAllocation)
        free(*(cur++));
    free(*lastAllocation);
    lastAllocation = allocations;
}
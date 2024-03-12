#include "SafeMemoryAllocator.h"
#include "malloc.h"

void **allocations = 0;
int allocSize = 100;
int allocCount = 0;

void *SMalloc(size_t size)
{
    return SCalloc(1,size);
}

void *SCalloc(size_t num,size_t size)
{
    if(!allocations)
        allocations = calloc(allocSize, sizeof(void*));
    if(allocCount >= size) {
        allocations = realloc(allocations, allocSize * 2);
        allocSize *= 2;
    }
    allocations[allocCount] = calloc(num,size);
    ++allocCount;
    return allocations[allocCount-1];
}

void FreeAll()
{
    for(int i = 0; i < allocCount; ++i)
        free(allocations[i]);
    free(allocations);
}
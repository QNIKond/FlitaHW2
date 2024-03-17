#include "ArenaAllocator.h"
#include "malloc.h"
#define DEFAULTARENASIZE 16

#define T typeof(arena->elementSize)

void CreateArena(Arena *arena, int elemSize)
{
    arena->elementSize = elemSize;
    arena->blockSize = DEFAULTARENASIZE;
    arena->memBlock = calloc(DEFAULTARENASIZE,elemSize);
    arena->filled = 0;
}

void ResizeArena(Arena *arena)
{
    do{
        arena->blockSize *= 2;
        arena->memBlock = realloc(arena->memBlock,arena->blockSize*arena->elementSize);

    }while(arena->filled >= arena->blockSize);
}

void *Alloc(Arena *arena, int* id)
{
    if(*id  == -1)
        *id = arena->filled++;
    else if (*id >= arena->filled)
        arena->filled = *id + 1;
    if(arena->filled >= arena->blockSize)
        ResizeArena(arena);
    return arena->memBlock + *id * arena->elementSize;
}

void FreeArena(Arena *arena)
{
    free(arena->memBlock);
}
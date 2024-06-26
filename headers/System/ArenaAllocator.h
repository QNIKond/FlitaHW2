#pragma once

typedef struct ARENA
{
    int elementSize;
    void* memBlock;
    int blockSize;
    int filled;
}Arena;

void CreateArena(Arena *arena, int elemSize);

void *Alloc(Arena *arena, int *id);

void ResetArena(Arena *arena);

void FreeArena(Arena *arena);
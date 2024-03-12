#pragma once
#include "malloc.h"

void *SMalloc(size_t size);

void *SCalloc(size_t num,size_t size);

void FreeAll();
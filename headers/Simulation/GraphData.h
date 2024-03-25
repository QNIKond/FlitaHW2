#pragma once
#include "raylib.h"
typedef struct GRAPHCONFIG{
    int globalIterations;
    int perCycleIterations;
    int isRunning;
    Rectangle bounds;
}GraphConfig;

void InitializeGraphConfig(GraphConfig *gc);
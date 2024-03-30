#pragma once
#include "raylib.h"
typedef struct GRAPHCONFIG{
    int globalIterations;
    int perCycleIterations;
    int isRunning;



    int verticesCount;
    int edgesCount;

    float K;
    float heat;

    //Constants
    Rectangle bounds;
    float edgeResizeConst;
    float C;
    float superNodeRatio;
    int maxTreeDepth;
}GraphConfig;

void InitializeGraphConfig(GraphConfig *gc);
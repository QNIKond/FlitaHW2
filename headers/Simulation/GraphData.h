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

    int useQTOptimization;
    int showDots;
    int showEdgeWeights;
}GraphConfig;

void InitializeGraphConfig(GraphConfig *gc);
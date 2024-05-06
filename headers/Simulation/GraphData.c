#include "GraphData.h"

void InitializeGraphConfig(GraphConfig *gc){
    gc->isRunning = 0;
    gc->globalIterations = 0;
    gc->perCycleIterations = 6;
    gc->bounds = (Rectangle){0,0,1200,1200};
    gc->verticesCount = 0;
    gc->edgesCount = 0;
    gc->K = 0;
    gc->heat = 1;
    gc->edgeResizeConst = 0.2f;
    gc->C = 0.2f;
    gc->superNodeRatio = 1.2f;
    gc->maxTreeDepth = 8;
    gc->useQTOptimization = 1;
    gc->showDots = 1;
    gc->showEdgeWeights = 0;
    gc->components = 0;
}
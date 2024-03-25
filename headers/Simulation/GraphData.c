#include "GraphData.h"

void InitializeGraphConfig(GraphConfig *gc){
    gc->isRunning = 0;
    gc->globalIterations = 0;
    gc->perCycleIterations = 1;
    gc->bounds = (Rectangle){0,0,1200,700};
}
#pragma once
#include "../Simulation/Graph.h"
#include "../Simulation/GraphData.h"

typedef enum
{
    GEMMoveCamera,
    GEMMoveVertices,
    GEMEditVertices,
    GEMEditEdges
} GraphEditMode;

void UpdateDrawGraphWindow(Graph* graph,GraphConfig *gc, int* focus);

GraphEditMode *GetGraphEditMode();

#pragma once
#include "../Simulation/Graph.h"

typedef enum
{
    GEMMoveCamera,
    GEMMoveVertices,
    GEMEditVertices,
    GEMEditEdges
} GraphEditMode;

void UpdateDrawGraphWindow(Graph* graph, int* focus);

GraphEditMode *GetGraphEditMode();

#pragma once
#include "../Simulation/Graph.h"
#include "../Simulation/GraphData.h"

Graph* GetDefaultGraph();

Graph *OpenMtx(GraphConfig *gc);

Graph *OpenEdgesList(GraphConfig *gc);

Graph* OpenAdjacencyMatrix(GraphConfig *gc);
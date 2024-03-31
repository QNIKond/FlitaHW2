#pragma once
#include "../Simulation/Graph.h"
#include "../Simulation/GraphData.h"

Graph *OpenMtx(Graph *graph, GraphConfig *gc);

Graph *OpenEdgesList(Graph *graph, GraphConfig *gc);

Graph* OpenAdjacencyMatrix(Graph *graph, GraphConfig *gc);
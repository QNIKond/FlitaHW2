#pragma once
#include "Graph.h"
typedef int qtID;
typedef struct QUADTREE
{
    nodeID node;
    Vector2 massCenter;
    int mass;
    qtID children[4];
    int isLeaf;
}QuadTree;

void BuildQuadTree(Graph *graph,Rectangle bounds, int maxDepth);
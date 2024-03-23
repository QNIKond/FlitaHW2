#pragma once
#include "raylib.h"
#include "../ArenaAllocator.h"
//#include "QuadTree.h"
struct QUADTREE;
struct GRAPHEDGE;
typedef int nodeID;
typedef int edgeID;

typedef struct GRAPHNODE
{
    edgeID edges;
    Vector2 pos;
    unsigned int state;
}GraphNode;

#define EOEDGELIST -1
typedef struct GRAPHEDGE
{
    nodeID node;
    edgeID nextEdge;
    unsigned char state;
}GraphEdge;

typedef struct GRAPH
{
    Arena nodes;
    Arena edges;
    Arena qtree;
} Graph;

#define GETNODES(X) ((GraphNode*)X->nodes.memBlock)
#define GETEDGES(X) ((GraphEdge*)X->edges.memBlock)
#define GETQTNODES(X) ((QuadTree*)X->qtree.memBlock)

void InitializeGraph(Graph* graph);

void PlaceNewNode(Graph *graph, Vector2 pos);

void DeleteNode(Graph* graph, nodeID node);

void FlipNodesConnection(Graph *graph, nodeID node1, nodeID node2);

nodeID FindNodeByPosition(Graph *graph, Vector2 point);

void DestroyGraph(Graph *graph);
#pragma once
#include "raylib.h"
#include "../System/ArenaAllocator.h"
//#include "QuadTree.h"
struct QUADTREE;
struct GRAPHEDGE;
typedef int nodeID;
typedef int edgeID;

#define GS_MIS 4
#define GS_VISITED 2
#define GS_EXISTS 1

typedef struct GRAPHNODE
{
    edgeID edges;
    Vector2 pos;
    unsigned int state;
    nodeID parent;
}GraphNode;

#define EOEDGELIST -1
typedef struct GRAPHEDGE
{
    nodeID node;
    edgeID nextEdge;
    unsigned char state;
    int weight;
}GraphEdge;

typedef struct GRAPH
{
    Arena nodes;
    Arena edges;
    Arena qtree;
    struct GRAPH *finerGraph;
    struct GRAPH *coarserGraph;
} Graph;

#define GETNODES(X) ((GraphNode*)X->nodes.memBlock)
#define GETEDGES(X) ((GraphEdge*)X->edges.memBlock)
#define GETQTNODES(X) ((QuadTree*)X->qtree.memBlock)

void InitializeGraph(Graph* graph);

void PlaceNewNode(Graph *graph, Vector2 pos);

int CreateNode(Graph *graph, nodeID id);

void DeleteNode(Graph* graph, nodeID node);

void CreateNodeConnection(Graph *graph, nodeID node1, nodeID node2);

void AddWeight(Graph *graph, nodeID node1, nodeID node2, int weight);

void DisconnectNodes(Graph *graph, nodeID node1, nodeID node2);

nodeID FindNodeByPosition(Graph *graph, Vector2 point, float radius);

void ShuffleNodes(Graph *graph,Rectangle bounds);

void ResetGraph(Graph *graph);

void DestroyGraph(Graph *graph);
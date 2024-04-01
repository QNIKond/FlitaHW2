#include "Graph.h"
#include "QuadTree.h"
#include <stdlib.h>

void InitializeGraph(Graph* graph)
{
    CreateArena(&graph->nodes,sizeof(GraphNode));
    CreateArena(&graph->edges,sizeof(GraphEdge));
    CreateArena(&graph->qtree,sizeof(QuadTree));
    graph->finerGraph = graph;
    graph->coarserGraph = graph;
}

edgeID CreateEdges(Graph *graph, nodeID node,nodeID* neighbours, int neighboursCount);

void PlaceNewNode(Graph *graph, Vector2 pos)
{
    int id = -1;
    *(GraphNode*)Alloc(&graph->nodes,&id) = (GraphNode){EOEDGELIST,pos,1,-1,1};
}

int CreateNode(Graph *graph, nodeID id)
{
    *(GraphNode*)Alloc(&graph->nodes,&id) = (GraphNode){EOEDGELIST, {0,0},1,-1,1};
    return id;
}

void DeleteNode(Graph* graph, nodeID node)
{
    if(node >= 0)
        GETNODES(graph)[node].state = 0;
}

edgeID AddEdge(Graph* graph, nodeID source, nodeID dest)
{
    edgeID newEdge = -1;
    edgeID curEdge = GETNODES(graph)[source].edges;
    *(GraphEdge*)Alloc(&graph->edges, &newEdge) = (GraphEdge){dest, EOEDGELIST, 1,1};
    if(curEdge == -1)
        GETNODES(graph)[source].edges = newEdge;
    else {
        while (GETEDGES(graph)[curEdge].nextEdge != EOEDGELIST)
            curEdge = GETEDGES(graph)[curEdge].nextEdge;
        GETEDGES(graph)[curEdge].nextEdge = newEdge;
    }
    return newEdge;
}

edgeID GetEdge(Graph *graph, nodeID node1, nodeID node2)
{
    edgeID curEdge = GETNODES(graph)[node1].edges;
    while (curEdge != EOEDGELIST) {
        if(GETEDGES(graph)[curEdge].node == node2)
            return curEdge;
        curEdge = GETEDGES(graph)[curEdge].nextEdge;
    }
    return -1;
}

void CreateNodeConnection(Graph *graph, nodeID node1, nodeID node2)
{
    if(node1 == node2)
        return;
    edgeID edge = GetEdge(graph, node1, node2);
    if(edge != -1){
        GETEDGES(graph)[edge].state |= GS_EXISTS;
        GETEDGES(graph)[edge].weight = 1;
        edge = GetEdge(graph, node2, node1);
        GETEDGES(graph)[edge].state |= GS_EXISTS;
        GETEDGES(graph)[edge].weight = 1;
    }
    else {
        AddEdge(graph, node1, node2);
        AddEdge(graph, node2, node1);
    }
}

void AddWeight(Graph *graph, nodeID node1, nodeID node2, int weight){
    if(node1 == node2)
        return;
    edgeID edge1 = GetEdge(graph, node1, node2);
    edgeID edge2;
    if(edge1 == -1){
        edge1 = AddEdge(graph, node1, node2);
        edge2 = AddEdge(graph, node2, node1);
    }
    else {
        edge2 = GetEdge(graph, node2, node1);
        GETEDGES(graph)[edge1].weight += weight;
        GETEDGES(graph)[edge2].weight += weight;
    }
}

void DisconnectNodes(Graph *graph, nodeID node1, nodeID node2){
    if(node1 == node2)
        return;
    edgeID edge = GetEdge(graph, node1, node2);
    if(edge != -1){
        GETEDGES(graph)[edge].state = 0;
        edge = GetEdge(graph, node2, node1);
        GETEDGES(graph)[edge].state = 0;
    }
}

#define ACCURACY 10
#define ABS(X) ((X)>=0 ? (X) : (-(X)))
nodeID FindNodeByPosition(Graph *graph, Vector2 point, float radius)
{
    for (int i = 0; i < graph->nodes.filled; ++i) {
        if((ABS(GETNODES(graph)[i].pos.x - point.x) < radius) &&
                (ABS(GETNODES(graph)[i].pos.y - point.y) < radius) &&GETNODES(graph)[i].state)
            return i;
    }
    return -1;
}

void ShuffleNodes(Graph *graph,Rectangle bounds){
    for(int i = 0; i < graph->nodes.filled; ++i)
    GETNODES(graph)[i].pos = (Vector2){bounds.x + (float)(rand()%(int)bounds.width),
                                       bounds.y + (float)(rand()%(int)bounds.height)};
}


void RecursevlyDestroyGraphs(Graph *graph)
{
    FreeArena(&graph->nodes);
    FreeArena(&graph->edges);
    FreeArena(&graph->qtree);
    if(graph->coarserGraph) {
        RecursevlyDestroyGraphs(graph->coarserGraph);
        free(graph->coarserGraph);
    }
}

void DestroySubgraphs(Graph *graph)
{
    if(graph->coarserGraph == graph)
        return;
    graph->finerGraph->coarserGraph = 0;
    RecursevlyDestroyGraphs(graph->coarserGraph);
    free(graph->coarserGraph);
    graph->finerGraph = graph;
    graph->coarserGraph = graph;
}

void ResetGraph(Graph *graph){
    ResetArena(&graph->nodes);
    ResetArena(&graph->edges);
    ResetArena(&graph->qtree);
    DestroySubgraphs(graph);
}

void DestroyGraph(Graph *graph)
{
    graph->finerGraph->coarserGraph = 0;
    RecursevlyDestroyGraphs(graph);
}
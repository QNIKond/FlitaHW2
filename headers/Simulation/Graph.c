#include "Graph.h"
#include "QuadTree.h"
#include <stdlib.h>

void InitializeGraph(Graph* graph)
{
    CreateArena(&graph->nodes,sizeof(GraphNode));
    CreateArena(&graph->edges,sizeof(GraphEdge));
    CreateArena(&graph->qtree,sizeof(QuadTree));
}

edgeID CreateEdges(Graph *graph, nodeID node,nodeID* neighbours, int neighboursCount);

void PlaceNewNode(Graph *graph, Vector2 pos)
{
    int id = -1;
    *(GraphNode*)Alloc(&graph->nodes,&id) = (GraphNode){EOEDGELIST,pos,1};
}

void CreateNode(Graph *graph, nodeID id)
{
    *(GraphNode*)Alloc(&graph->nodes,&id) = (GraphNode){EOEDGELIST, {0,0},1};
}

void DeleteNode(Graph* graph, nodeID node)
{
    if(node >= 0)
        GETNODES(graph)[node].state = 0;
}

void GetLastEdge(Graph *graph, edgeID edge)
{

}

void AddEdge(Graph* graph, nodeID source, nodeID dest)
{
    edgeID newEdge = -1;
    edgeID curEdge = GETNODES(graph)[source].edges;
    *(GraphEdge*)Alloc(&graph->edges, &newEdge) = (GraphEdge){dest, EOEDGELIST, 1};
    if(curEdge == -1)
        GETNODES(graph)[source].edges = newEdge;
    else {
        while (GETEDGES(graph)[curEdge].nextEdge != EOEDGELIST)
            curEdge = GETEDGES(graph)[curEdge].nextEdge;
        GETEDGES(graph)[curEdge].nextEdge = newEdge;
    }

}

int SetEdge(Graph *graph, nodeID node1, nodeID node2, int state)
{
    edgeID curEdge = GETNODES(graph)[node1].edges;
    while (curEdge != EOEDGELIST) {
        if(GETEDGES(graph)[curEdge].node == node2)
        {
            GETEDGES(graph)[curEdge].state = state;
            return 1;
        }
        curEdge = GETEDGES(graph)[curEdge].nextEdge;
    }
    return 0;
}

void ConnectNodes(Graph *graph, nodeID node1, nodeID node2)
{
    if(node1 == node2)
        return;
    if(SetEdge(graph, node1, node2,1)){
        SetEdge(graph, node2, node1,1);
    }
    else {
        AddEdge(graph, node1, node2);
        AddEdge(graph, node2, node1);
    }
}

void DisconnectNodes(Graph *graph, nodeID node1, nodeID node2){
    if(node1 == node2)
        return;
    if(SetEdge(graph, node1, node2,0)){
        SetEdge(graph, node2, node1,0);
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

void ResetGraph(Graph *graph){
    ResetArena(&graph->nodes);
    ResetArena(&graph->edges);
    ResetArena(&graph->qtree);
}

void DestroyGraph(Graph *graph)
{
    FreeArena(&graph->nodes);
    FreeArena(&graph->edges);
    FreeArena(&graph->qtree);
}
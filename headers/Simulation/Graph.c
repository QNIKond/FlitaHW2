#include "Graph.h"
#include "malloc.h"

void InitializeGraph(Graph* graph)
{
    CreateArena(&graph->nodes,sizeof(GraphNode));
    CreateArena(&graph->edges,sizeof(GraphEdge));
}

edgeID CreateEdges(Graph *graph, nodeID node,nodeID* neighbours, int neighboursCount);

void PlaceNewNode(Graph *graph, Vector2 pos)
{
    int id = -1;
    *(GraphNode*)Alloc(&graph->nodes,&id) = (GraphNode){EOEDGELIST,pos,1};
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

int FlipEdge(Graph *graph, nodeID node1, nodeID node2)
{
    edgeID curEdge = GETNODES(graph)[node1].edges;
    while (curEdge != EOEDGELIST) {
        if(GETEDGES(graph)[curEdge].node == node2)
        {
            GETEDGES(graph)[curEdge].state ^= 1;
            return 1;
        }
        curEdge = GETEDGES(graph)[curEdge].nextEdge;
    }
    return 0;
}

void FlipNodesConnection(Graph *graph, nodeID node1, nodeID node2)
{
    if(node1 == node2)
        return;
    if(FlipEdge(graph,node1,node2)){
        FlipEdge(graph,node2,node1);
    }
    else {
        AddEdge(graph, node1, node2);
        AddEdge(graph, node2, node1);
    }
}

#define ACCURACY 10
#define ABS(X) ((X)>=0 ? (X) : (-(X)))
nodeID FindNodeByPosition(Graph *graph, Vector2 point)
{
    for (int i = 0; i < graph->nodes.filled; ++i) {
        if((ABS(GETNODES(graph)[i].pos.x - point.x) < ACCURACY) &&
                (ABS(GETNODES(graph)[i].pos.y - point.y) < ACCURACY) &&GETNODES(graph)[i].state)
            return i;
    }
    return -1;
}

void DestroyGraph(Graph *graph)
{
    FreeArena(&graph->nodes);
    FreeArena(&graph->edges);
}
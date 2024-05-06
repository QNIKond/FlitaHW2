#include "Graph.h"
#include "QuadTree.h"
#include <stdlib.h>

void InitializeGraph(Graph* graph)
{
    CreateArena(&graph->nodes,sizeof(GraphNode));
    CreateArena(&graph->edges,sizeof(GraphEdge));
    CreateArena(&graph->qtree,sizeof(QuadTree));
    graph->finerGraph = 0;
    graph->coarserGraph = 0;
}

int AStar(Graph *graph, nodeID start, nodeID end);

nodeID PlaceNewNode(Graph *graph, Vector2 pos)
{
    nodeID id = -1;
    *(GraphNode*)Alloc(&graph->nodes,&id) = (GraphNode){EOEDGELIST,pos,1,-1,1};
    return id;
}

int CreateNode(Graph *graph, nodeID id)
{
    *(GraphNode*)Alloc(&graph->nodes,&id) = (GraphNode){EOEDGELIST, {0,0},1,-1,1};
    return id;
}

int DeleteNode(Graph* graph, nodeID node)
{
    if(node == -1)
        return 0;
    int components = 0;
    edgeID curEdge = GETNODES(graph)[node].edges;
    while(curEdge != -1){
        if(GETEDGES(graph)[curEdge].state&GS_EXISTS){
            nodeID curNode = GETEDGES(graph)[curEdge].node;
            if(GETNODES(graph)[curNode].state&GS_EXISTS){
                components -= DisconnectNodes(graph,node,curNode)-1;
            }
        }
        curEdge = GETEDGES(graph)[curEdge].nextEdge;
    }
    GETNODES(graph)[node].state = 0;

    return components-1;
}

edgeID AddEdge(Graph* graph, nodeID source, nodeID dest)
{
    edgeID newEdge = -1;
    edgeID curEdge = GETNODES(graph)[source].edges;
    *(GraphEdge*)Alloc(&graph->edges, &newEdge) = (GraphEdge){dest, EOEDGELIST, 1,1,0,50};
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

int CreateNodeConnection(Graph *graph, nodeID node1, nodeID node2)
{
    if(node1 == node2)
        return 1;
    int connected = AStar(graph,node1,node2);
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
    return connected;
}

void AddWeight(Graph *graph, nodeID node1, nodeID node2, int weight){
    if(node1 == node2)
        return;
    edgeID edge1 = GetEdge(graph, node1, node2);
    edgeID edge2;
    if(edge1 == -1){
        edge1 = AddEdge(graph, node1, node2);
        GETEDGES(graph)[edge1].weight = weight;
        edge2 = AddEdge(graph, node2, node1);
        GETEDGES(graph)[edge2].weight = weight;
    }
    else {
        edge2 = GetEdge(graph, node2, node1);
        GETEDGES(graph)[edge1].weight += weight;
        GETEDGES(graph)[edge2].weight += weight;
    }
}

int DisconnectNodes(Graph *graph, nodeID node1, nodeID node2){
    if(node1 == node2)
        return 1;
    edgeID edge = GetEdge(graph, node1, node2);
    if(edge != -1){
        GETEDGES(graph)[edge].state = 0;
        edge = GetEdge(graph, node2, node1);
        GETEDGES(graph)[edge].state = 0;
    }
    return AStar(graph, node1, node2);
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

void Mark(Graph *graph, nodeID start, nodeID end, int hue){
    edgeID edge;
    if(start > end)
        edge = GetEdge(graph, end, start);
    else
        edge = GetEdge(graph, start, end);
    GETEDGES(graph)[edge].colValue = 1;
    GETEDGES(graph)[edge].hue = (float)hue;
}

#define DIST(X,Y)(((X).x - (Y).x)*((X).x - (Y).x)+((X).y - (Y).y)*((X).y - (Y).y))
edgeID GetNextMinEdge(Graph *graph, nodeID start, nodeID end){
    edgeID curEdge = GETNODES(graph)[start].edges;
    if(curEdge == -1)
        return -1;
    int minDist = -1;
    edgeID minEdge = -1;
    while(curEdge != -1){
        if((GETEDGES(graph)[curEdge].state&GS_EXISTS)) {
            nodeID curNode = GETEDGES(graph)[curEdge].node;
            if ((GETNODES(graph)[curNode].state & (GS_EXISTS | GS_VISITED)) == GS_EXISTS)
            {
                int dist = DIST(GETNODES(graph)[curNode].pos,GETNODES(graph)[end].pos);
                if((dist < minDist) || (minDist == -1)){
                    minDist = dist;
                    minEdge = curEdge;
                }
            }
        }
        curEdge = GETEDGES(graph)[curEdge].nextEdge;
    }
    return minEdge;
}

int AStarRec(Graph *graph, nodeID start, nodeID end){
    if(start == end)
        return 1;
    edgeID curEdge = GetNextMinEdge(graph,start,end);
    while(curEdge != -1){
        nodeID curNode = GETEDGES(graph)[curEdge].node;
        GETNODES(graph)[curNode].state |= GS_VISITED;
        if(AStarRec(graph, curNode, end)) {
            Mark(graph, start, curNode, 0);
            return 1;
        }
        Mark(graph, start, curNode, 50);
        curEdge = GetNextMinEdge(graph,start,end);
    }
    return 0;
}

int AStar(Graph *graph, nodeID start, nodeID end){
    for(int i = 0; i < graph->nodes.filled; ++i){
        GETNODES(graph)[i].state -= GETNODES(graph)[i].state&GS_VISITED;
    }
    return AStarRec(graph,start,end);
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
    if(graph->coarserGraph == 0)
        return;
    RecursevlyDestroyGraphs(graph->coarserGraph);
    //graph->finerGraph = 0;
    graph->coarserGraph = 0;
}

void ResetGraph(Graph *graph){
    ResetArena(&graph->nodes);
    ResetArena(&graph->edges);
    ResetArena(&graph->qtree);
    DestroySubgraphs(graph);
}

void DestroyGraph(Graph *graph)
{
    RecursevlyDestroyGraphs(graph);
}
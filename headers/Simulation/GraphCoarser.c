#include "GraphCoarser.h"
#include "malloc.h"

void ConnectParents(Graph *graph, Graph *coarserGraph, nodeID node){
    nodeID parent = GETNODES(graph)[node].parent;
    edgeID curEdge = GETNODES(graph)[node].edges;
    nodeID inc;
    while(curEdge != -1){
        inc = GETEDGES(graph)[curEdge].node;
        if((GETNODES(graph)[inc].state&GS_EXISTS) && (GETNODES(graph)[inc].parent != -1) &&
        (GETNODES(graph)[inc].parent != parent))
            AddWeight(coarserGraph, parent, GETNODES(graph)[inc].parent, GETEDGES(graph)[curEdge].weight);
        curEdge = GETEDGES(graph)[curEdge].nextEdge;
    }
}

void CoarseGraph(Graph *graph){
    Graph *coarserGraph = malloc(sizeof(Graph));
    InitializeGraph(coarserGraph);
    graph->coarserGraph->finerGraph = coarserGraph;
    coarserGraph->coarserGraph = graph->coarserGraph;
    graph->coarserGraph = coarserGraph;
    coarserGraph->finerGraph = graph;
    for(int i = 0; i < graph->nodes.filled; ++i)
        if((GETNODES(graph)[i].state&(GS_EXISTS | GS_VISITED)) == GS_EXISTS){
            GETNODES(graph)[i].state |= GS_VISITED;
            edgeID curEdge = GETNODES(graph)[i].edges;
            nodeID inc = -1;
            if(curEdge != -1)
                inc = GETEDGES(graph)[curEdge].node;
            int t = GETNODES(graph)[inc].state;
            while((curEdge != -1) && ((GETNODES(graph)[inc].state&(GS_EXISTS | GS_VISITED)) != GS_EXISTS)){
                curEdge = GETEDGES(graph)[curEdge].nextEdge;
                inc = GETEDGES(graph)[curEdge].node;
            }
            nodeID parent = -1;
            parent = CreateNode(coarserGraph, parent);
            GETNODES(graph)[i].parent = parent;
            if(curEdge != -1){
                GETNODES(graph)[inc].state |= GS_VISITED;
                GETNODES(graph)[inc].parent = parent;
                ConnectParents(graph, coarserGraph, inc);
            }
            ConnectParents(graph, coarserGraph, i);
        }
}
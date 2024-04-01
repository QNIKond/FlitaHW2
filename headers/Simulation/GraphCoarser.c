#include "GraphCoarser.h"
#include "malloc.h"

void ConnectNeighboursToParent(Graph *graph, Graph *coarserGraph, nodeID node){
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

void CoarseEC(Graph *graph){
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
            int maxWeight = 0;
            edgeID maxEdge = -1;
            while(curEdge != -1){
                inc = GETEDGES(graph)[curEdge].node;
                if((maxWeight < (GETNODES(graph)[inc].weight)) &&
                ((GETNODES(graph)[inc].state&(GS_EXISTS | GS_VISITED)) == GS_EXISTS)){
                    maxWeight = GETNODES(graph)[inc].weight;
                    maxEdge = curEdge;
                }
                curEdge = GETEDGES(graph)[curEdge].nextEdge;
            }
            curEdge = maxEdge;
            inc = GETEDGES(graph)[curEdge].node;
            nodeID parent = -1;
            parent = CreateNode(coarserGraph, parent);
            GETNODES(coarserGraph)[parent].weight += GETNODES(graph)[i].weight - 1;
            GETNODES(graph)[i].parent = parent;
            if(curEdge != -1){
                GETNODES(graph)[inc].state |= GS_VISITED;
                GETNODES(graph)[inc].parent = parent;
                GETNODES(coarserGraph)[parent].weight += GETNODES(graph)[inc].weight;
                ConnectNeighboursToParent(graph, coarserGraph, inc);
            }
            ConnectNeighboursToParent(graph, coarserGraph, i);
        }
}

/*void SearchForParentNeighbours(Graph *graph, Graph *coarserGraph, nodeID node){
    nodeID parent = GETNODES(graph)[node].parent;
    edgeID curEdge = GETNODES(graph)[node].edges;
    nodeID inc;
    while(curEdge != -1){
        if
        curEdge = GETEDGES(graph)[curEdge].nextEdge;
    }
}*/

void CoarseMIVS(Graph *graph){
    Graph *coarserGraph = malloc(sizeof(Graph));
    InitializeGraph(coarserGraph);
    graph->coarserGraph->finerGraph = coarserGraph;
    coarserGraph->coarserGraph = graph->coarserGraph;
    graph->coarserGraph = coarserGraph;
    coarserGraph->finerGraph = graph;
    for(int i = 0; i < graph->nodes.filled; ++i)
        if((GETNODES(graph)[i].state&(GS_EXISTS | GS_VISITED)) == GS_EXISTS) {
            GETNODES(graph)[i].state |= GS_VISITED;
            nodeID parent = -1;
            parent = CreateNode(coarserGraph, parent);
            GETNODES(coarserGraph)[parent].weight += GETNODES(graph)[i].weight - 1;
            GETNODES(graph)[i].parent = parent;
            edgeID curEdge = GETNODES(graph)[i].edges;
            while(curEdge != -1){
                nodeID inc = GETEDGES(graph)[curEdge].node;
                if(GETNODES(graph)[inc].state&GS_EXISTS){
                    GETNODES(graph)[inc].state |= GS_VISITED;
                    if(GETNODES(graph)[inc].parent == -1){
                        GETNODES(graph)[inc].parent = parent;
                        GETNODES(coarserGraph)[parent].weight += GETNODES(graph)[inc].weight;
                        ConnectNeighboursToParent(graph,coarserGraph,inc);
                    }
                    else{
                        AddWeight(coarserGraph,parent,GETNODES(graph)[inc].parent,
                                  GETEDGES(graph)[curEdge].weight);
                    }
                }
                curEdge = GETEDGES(graph)[curEdge].nextEdge;
            }
        }
}
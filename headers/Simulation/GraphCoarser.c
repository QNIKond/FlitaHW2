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

void PrepareGraph(Graph *graph){
    for(int i = 0; i < graph->nodes.filled; ++i){
        GETNODES(graph)[i].state -= GETNODES(graph)[i].state&GS_VISITED;
        GETNODES(graph)[i].parent = -1;
    }
    if(graph->coarserGraph)
        DestroySubgraphs(graph);
    Graph *coarserGraph = malloc(sizeof(Graph));
    InitializeGraph(coarserGraph);
    graph->coarserGraph = coarserGraph;
    coarserGraph->finerGraph = graph;
}

void CoarseEC(Graph *graph){
    PrepareGraph(graph);
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
            parent = PlaceNewNode(graph->coarserGraph, GETNODES(graph)[i].pos);
            GETNODES(graph->coarserGraph)[parent].weight += GETNODES(graph)[i].weight - 1;
            GETNODES(graph)[i].parent = parent;
            if(curEdge != -1){
                GETNODES(graph)[inc].state |= GS_VISITED;
                GETNODES(graph)[inc].parent = parent;
                GETNODES(graph->coarserGraph)[parent].weight += GETNODES(graph)[inc].weight;
                ConnectNeighboursToParent(graph, graph->coarserGraph, inc);
            }
            ConnectNeighboursToParent(graph, graph->coarserGraph, i);
        }
}

void CoarseMIVS(Graph *graph){
    PrepareGraph(graph);
    for(int i = 0; i < graph->nodes.filled; ++i)
        if((GETNODES(graph)[i].state&(GS_EXISTS | GS_VISITED)) == GS_EXISTS) {
            GETNODES(graph)[i].state |= GS_VISITED;
            nodeID parent = PlaceNewNode(graph->coarserGraph, GETNODES(graph)[i].pos);
            //parent = CreateNode(coarserGraph, parent);

            GETNODES(graph->coarserGraph)[parent].weight += GETNODES(graph)[i].weight - 1;
            GETNODES(graph)[i].parent = parent;
            edgeID curEdge = GETNODES(graph)[i].edges;
            while(curEdge != -1){
                nodeID inc = GETEDGES(graph)[curEdge].node;
                if(GETNODES(graph)[inc].state&GS_EXISTS){
                    GETNODES(graph)[inc].state |= GS_VISITED;
                    if(GETNODES(graph)[inc].parent == -1){
                        GETNODES(graph)[inc].parent = parent;
                        GETNODES(graph->coarserGraph)[parent].weight += GETNODES(graph)[inc].weight;
                        ConnectNeighboursToParent(graph,graph->coarserGraph,inc);
                    }
                    else{
                        AddWeight(graph->coarserGraph,parent,GETNODES(graph)[inc].parent,
                                  GETEDGES(graph)[curEdge].weight);
                    }
                }
                curEdge = GETEDGES(graph)[curEdge].nextEdge;
            }
        }
}

void RefineGraph(Graph* graph){
    if(graph->coarserGraph == 0)
        return;
    for(int i = 0; i< graph->nodes.filled; ++i)
        if(GETNODES(graph)[i].state&GS_EXISTS){
            Vector2 pos = GETNODES(graph->coarserGraph)[GETNODES(graph)[i].parent].pos;
            pos.x += rand()%20 - 10;
            pos.y += rand()%20 - 10;
            GETNODES(graph)[i].pos = pos;
        }
}
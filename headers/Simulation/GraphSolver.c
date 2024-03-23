#include "GraphSolver.h"
#include "QuadTree.h"
Graph* graph;

void SetNewGraph(Graph* gr)
{
    graph = gr;
}

void Cycle(GraphConfig *gc)
{
    for(int i = 0; i < graph->nodes.filled; ++i){
        GETNODES(graph)[i].pos.y += 1;
        if(GETNODES(graph)[i].pos.y > gc->bounds.height)
            GETNODES(graph)[i].pos.y = 0;
    }

}

void SolveGraph(GraphConfig *gc)
{
    if(!IsKeyDown(KEY_S))
        for(int i = 0; i < gc->globalIterations+gc->perCycleIterations; ++i)
            Cycle(gc);
    gc->globalIterations = 0;
    //if(IsKeyPressed(KEY_V))
        BuildQuadTree(graph,gc->bounds,8);
}
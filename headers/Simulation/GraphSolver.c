#include "GraphSolver.h"
#include "QuadTree.h"
#include <math.h>
Graph* graph;

void SetNewGraph(Graph* gr)
{
    graph = gr;
}
#define C 0.2f
#define K 100
float GetMod(Vector2 vec1, Vector2 vec2){
    return (float)(sqrt((vec1.x - vec2.x)*(vec1.x - vec2.x) + (vec1.y - vec2.y)*(vec1.y - vec2.y)));
}

void ApplyForces(GraphConfig* gc, nodeID node){
    Vector2 force = {0,0};
    Vector2 nodePos = GETNODES(graph)[node].pos;
    Vector2 srcPos;
    float mod;

    for(int i = 0; i < graph->nodes.filled; ++i)
        if((i != node) && (GETNODES(graph)[i].state)){
            srcPos = GETNODES(graph)[i].pos;
            mod = GetMod(nodePos,srcPos);
            mod *= mod;
            if(mod > 0) {
                force.x += C * K * K * (nodePos.x - srcPos.x) / mod;
                force.y += C * K * K * (nodePos.y - srcPos.y) / mod;
            }
    }
    edgeID edge = GETNODES(graph)[node].edges;
    while(edge != EOEDGELIST){
        if(GETEDGES(graph)[edge].state && GETNODES(graph)[GETEDGES(graph)[edge].node].state) {
            srcPos = GETNODES(graph)[GETEDGES(graph)[edge].node].pos;
            mod = GetMod(nodePos, srcPos);
            if (mod > 0) {
                force.x += mod / K * (srcPos.x - nodePos.x);
                force.y += mod / K * (srcPos.y - nodePos.y);
            }
        }
        edge = GETEDGES(graph)[edge].nextEdge;
    }
    /*Vector2 center = (Vector2){gc->bounds.width/2+gc->bounds.x,gc->bounds.height/2+gc->bounds.y};
    mod = GetMod(nodePos,center);
    force.x += (center.x-nodePos.x)/mod;
    force.y += (center.y-nodePos.y)/mod;*/
    mod = GetMod(force,(Vector2){0,0});
    if(mod > 0) {
        GETNODES(graph)[node].pos.x += force.x / mod;
        GETNODES(graph)[node].pos.y += force.y / mod;
    }
}

void Bound(GraphConfig *gc,nodeID node){
    if(GETNODES(graph)[node].pos.x<gc->bounds.x)
        GETNODES(graph)[node].pos.x=gc->bounds.x;
    if(GETNODES(graph)[node].pos.y<gc->bounds.y)
        GETNODES(graph)[node].pos.y=gc->bounds.y;
    if(GETNODES(graph)[node].pos.x>gc->bounds.x+gc->bounds.width)
        GETNODES(graph)[node].pos.x=gc->bounds.x+gc->bounds.width;
    if(GETNODES(graph)[node].pos.y>gc->bounds.y+gc->bounds.height)
        GETNODES(graph)[node].pos.y=gc->bounds.y+gc->bounds.height;
}

void Cycle(GraphConfig *gc)
{
    for(int i = 0; i < graph->nodes.filled; ++i){
        if(GETNODES(graph)[i].state) {
            ApplyForces(gc, i);
            Bound(gc, i);
        }
    }

}

void SolveGraph(GraphConfig *gc)
{
    if(IsKeyDown(KEY_S))
        for(int i = 0; i < gc->globalIterations+gc->perCycleIterations; ++i)
            Cycle(gc);
    gc->globalIterations = 0;
    //if(IsKeyPressed(KEY_V))
        BuildQuadTree(graph,gc->bounds,8);
}
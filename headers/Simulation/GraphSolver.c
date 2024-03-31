#include "GraphSolver.h"
#include "QuadTree.h"
#include <math.h>

float GetDist(Vector2 vec1, Vector2 vec2){
    return (float)(sqrt((vec1.x - vec2.x)*(vec1.x - vec2.x) + (vec1.y - vec2.y)*(vec1.y - vec2.y)));
}

Vector2 GetRepulsiveForse(Graph *graph,GraphConfig* gc, Vector2 point, qtID node, int size){
    Vector2 force = {0,0};
    Vector2 source = GETQTNODES(graph)[node].massCenter;
    float dist = GetDist(point, source);
    if(((size/dist)<=gc->superNodeRatio)||(GETQTNODES(graph)[node].isLeaf)){
        dist *= dist;
        if(dist > 0) {
            force.x += gc->C * gc->K * gc->K * (point.x - source.x) / dist * GETQTNODES(graph)[node].mass;
            force.y += gc->C * gc->K * gc->K * (point.y - source.y) / dist * GETQTNODES(graph)[node].mass;
        }
        if((GETQTNODES(graph)[node].isLeaf)&&(GETQTNODES(graph)[node].children[0] != -1)){
            Vector2 childForce = GetRepulsiveForse(graph, gc, point,
                                                   GETQTNODES(graph)[node].children[0],size/2);
            force.x += childForce.x;
            force.y += childForce.y;
        }
    }
    else{
        for(int i = 0; i < 4; ++i)
        if(GETQTNODES(graph)[node].children[i] != -1){
            Vector2 childForce = GetRepulsiveForse(graph, gc, point,
                                                   GETQTNODES(graph)[node].children[i],size/2);
            force.x += childForce.x;
            force.y += childForce.y;
        }
    }
    return force;
}

void ApplyForces(Graph *graph,GraphConfig* gc, nodeID node){
    Vector2 nodePos = GETNODES(graph)[node].pos;
    Vector2 srcPos;

    float dist;
    Vector2 force = {0,0};
    if(gc->useQTOptimization)
        force = GetRepulsiveForse(graph, gc, GETNODES(graph)[node].pos, 0,gc->bounds.width);
    else
        for(int i = 0; i < graph->nodes.filled; ++i)
            if((i != node) && (GETNODES(graph)[i].state)){
                srcPos = GETNODES(graph)[i].pos;
                dist = GetDist(nodePos, srcPos);
                dist *= dist;
                if(dist > 0) {
                    force.x += gc->C * gc->K * gc->K * (nodePos.x - srcPos.x) / dist;
                    force.y += gc->C * gc->K * gc->K * (nodePos.y - srcPos.y) / dist;
                }
        }
    edgeID edge = GETNODES(graph)[node].edges;
    while(edge != EOEDGELIST){
        if(GETEDGES(graph)[edge].state && GETNODES(graph)[GETEDGES(graph)[edge].node].state) {
            srcPos = GETNODES(graph)[GETEDGES(graph)[edge].node].pos;
            dist = GetDist(nodePos, srcPos);
            if (dist > 0) {
                force.x += dist / gc->K * (srcPos.x - nodePos.x);
                force.y += dist / gc->K * (srcPos.y - nodePos.y);
            }
        }
        edge = GETEDGES(graph)[edge].nextEdge;
    }


    dist = GetDist(force, (Vector2) {0, 0});
    if(dist > 0) {
        GETNODES(graph)[node].pos.x += gc->heat * force.x / dist;
        GETNODES(graph)[node].pos.y += gc->heat * force.y / dist;
    }

    GETNODES(graph)[node].pos.x +=gc->bounds.width/2 - GETQTNODES(graph)[0].massCenter.x;
    GETNODES(graph)[node].pos.y +=gc->bounds.height/2 - GETQTNODES(graph)[0].massCenter.y;
}

void Bound(Graph *graph,GraphConfig *gc,nodeID node){
    if(GETNODES(graph)[node].pos.x<gc->bounds.x)
        GETNODES(graph)[node].pos.x=gc->bounds.x;
    if(GETNODES(graph)[node].pos.y<gc->bounds.y)
        GETNODES(graph)[node].pos.y=gc->bounds.y;
    if(GETNODES(graph)[node].pos.x>gc->bounds.x+gc->bounds.width)
        GETNODES(graph)[node].pos.x=gc->bounds.x+gc->bounds.width;
    if(GETNODES(graph)[node].pos.y>gc->bounds.y+gc->bounds.height)
        GETNODES(graph)[node].pos.y=gc->bounds.y+gc->bounds.height;
}

void Cycle(Graph *graph,GraphConfig *gc)
{
    for(int i = 0; i < graph->nodes.filled; ++i){
        if(GETNODES(graph)[i].state) {
            ApplyForces(graph, gc, i);
            Bound(graph, gc, i);
        }
    }

}

void CountNodesAndEdges(Graph *graph, GraphConfig *gc){
    gc->verticesCount = 0;
    gc->edgesCount = 0;
    for(int i = 0; i < graph->nodes.filled; ++i) {
        if (GETNODES(graph)[i].state) {
            ++gc->verticesCount;
            edgeID curEdge = GETNODES(graph)[i].edges;
            while (curEdge != EOEDGELIST) {
                if ((GETEDGES(graph)[curEdge].node > i) && (GETEDGES(graph)[curEdge].state & 1) &&
                    (GETNODES(graph)[GETEDGES(graph)[curEdge].node].state & 1))
                    ++gc->edgesCount;
                curEdge = GETEDGES(graph)[curEdge].nextEdge;
            }
        }
    }
}

void SolveGraph(Graph *graph, GraphConfig *gc)
{
    CountNodesAndEdges(graph, gc);
    if(!gc->isRunning)
        return;
    BuildQuadTree(graph,gc->bounds,gc->maxTreeDepth);
    gc->K = gc->edgeResizeConst * sqrt(gc->bounds.width * gc->bounds.height / (gc->verticesCount + 1));

    for(int i = 0; i < gc->globalIterations+gc->perCycleIterations; ++i)
        Cycle(graph, gc);
    gc->globalIterations = 0;
    //if(IsKeyPressed(KEY_V))
}
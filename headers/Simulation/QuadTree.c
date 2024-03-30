#include "QuadTree.h"

#define TREENODE GETQTNODES(graph)[treeNode]

int GetChild(Vector2 pos, Rectangle center){
    return (pos.x>center.x) + (pos.y>center.y)*2;
}
void PlaceInList(Graph* graph,qtID list, nodeID node){
    while(GETQTNODES(graph)[list].children[0] != -1){
        list = GETQTNODES(graph)[list].children[0];
    }
    int id = -1;
    *(QuadTree*) Alloc(&graph->qtree,&id) = (QuadTree){node,{},0,{-1,-1,-1,-1},1};
    GETQTNODES(graph)[list].children[0] = id;
}

void PlaceNode(Graph* graph,qtID root, nodeID node,Rectangle center, int maxDepth)
{
    qtID curNode = root;
    Rectangle curCenter = center;
    int child = GetChild(GETNODES(graph)[node].pos,curCenter);
    int depth = 0;
    while(GETQTNODES(graph)[curNode].children[child] != -1){
        curNode = GETQTNODES(graph)[curNode].children[child];
        curCenter = (Rectangle){curCenter.x+curCenter.width*(float)((child%2)*2-1)
                ,curCenter.y+curCenter.height*(float)((child/2)*2 -1),
                                curCenter.width/2,curCenter.height/2};
        child = GetChild(GETNODES(graph)[node].pos,curCenter);
        ++depth;
        if(depth >= maxDepth){
            PlaceInList(graph, curNode,node);
            return;
        }
    }
    int id = -1;
    *(QuadTree*) Alloc(&graph->qtree,&id) = (QuadTree){node,{},0,{-1,-1,-1,-1},1};
    GETQTNODES(graph)[curNode].children[child] = id;
    if(GETQTNODES(graph)[curNode].isLeaf){
        GETQTNODES(graph)[curNode].isLeaf = 0;
        //PlaceNode(graph,root,GETQTNODES(graph)[curNode].node,bounds,maxDepth);
        PlaceNode(graph,curNode,GETQTNODES(graph)[curNode].node,curCenter,maxDepth-depth);
    }
}

Vector2 GetMedian(Vector2 pos1, int mass1,Vector2 pos2, int mass2){
    return (Vector2){(pos1.x*mass1 + pos2.x*mass2)/(mass1+mass2), (pos1.y*mass1+ pos2.y*mass2)/(mass1+mass2)};
}

void CalculateMassCenters(Graph *graph, qtID node){
    if(node == -1)
        return;
    Vector2 center = {0,0};
    int mass = 0;
    if(GETQTNODES(graph)[node].isLeaf){
        mass = 1;
        center = GETNODES(graph)[GETQTNODES(graph)[node].node].pos;
    }
    qtID childNode;
    for (int i = 0; i < 4; ++i) {
        childNode = GETQTNODES(graph)[node].children[i];
        if (childNode != -1) {
            CalculateMassCenters(graph, childNode);
            center = GetMedian(center, mass, GETQTNODES(graph)[childNode].massCenter,
                               GETQTNODES(graph)[childNode].mass);
            mass += GETQTNODES(graph)[childNode].mass;
        }
    }
    GETQTNODES(graph)[node].massCenter = center;
    GETQTNODES(graph)[node].mass = mass;
}

void BuildQuadTree(Graph *graph,Rectangle bounds, int maxDepth)
{
    graph->qtree.filled = 0;
    nodeID root = 0;
    while(!GETNODES(graph)[root].state){
        ++root;
        if(root >= graph->nodes.filled)
            return;
    }
    *(QuadTree *)Alloc(&graph->qtree,0) =
            (QuadTree){root,(Vector2){0,0},0,{-1,-1,-1,-1},1};
    for(int i = root+1; i < graph->nodes.filled; ++i)
        if(GETNODES(graph)[i].state)
            PlaceNode(graph,0,i,(Rectangle){bounds.width/2,bounds.height/2,bounds.width/4,bounds.height/4},maxDepth);
    CalculateMassCenters(graph,0);
}
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

void PlaceNode(Graph* graph,qtID root, nodeID node,Rectangle bounds, int maxDepth)
{
    qtID curNode = root;
    Rectangle curCenter = {bounds.width/2,bounds.height/2,bounds.width/4,bounds.height/4};
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
        PlaceNode(graph,root,GETQTNODES(graph)[curNode].node,bounds,maxDepth);
    }
}



void BuildQuadTree(Graph *graph,Rectangle bounds, int maxDepth)
{
    graph->qtree.filled = 0;
    *(QuadTree *)Alloc(&graph->qtree,0) =
            (QuadTree){0,(Vector2){0,0},0,{-1,-1,-1,-1},1};
    for(int i = 1; i < graph->nodes.filled; ++i)
        PlaceNode(graph,0,i,bounds,maxDepth);
}
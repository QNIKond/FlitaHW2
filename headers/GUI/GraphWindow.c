#include "GraphWindow.h"
#include "raylib.h"
#include "Layout.h"
#include "../Simulation/QuadTree.h"

GraphEditMode graphEditMode = GEMEditVertices;
int t = 1;
GraphEditMode *GetGraphEditMode()
{
    return &graphEditMode;
}
#define ROUND(X) ((int)X + ((int)(X*10)%10>=5))
//#define ROUND(X) (ROUND1((X/2))*2)
void DrawTree(Graph* graph, qtID leaf,Rectangle box)
{
    if(!graph->qtree.filled)
        return;
    Color color;
    if(GETQTNODES(graph)[leaf].isLeaf && (GETQTNODES(graph)[leaf].children[0] != -1))
        color = RED;
    else
        color = GREEN;

    if(IsKeyPressed(KEY_Q))
        t += 1;
    if(IsKeyPressed(KEY_W))
        t -= 1;
    if(IsKeyPressed(KEY_E))
        t = 1;

    DrawRectangleLinesEx((Rectangle){ROUND(box.x),ROUND(box.y),ROUND(box.width),ROUND(box.height)},1,color);
    //DrawRectangleLines(ROUND(box.x),ROUND(box.y),ROUND(box.width)+1,ROUND(box.height)+1,color);
    if(!GETQTNODES(graph)[leaf].isLeaf)
    {
        for(int i = 0; i < 2; ++i)
            for(int j = 0; j < 2; ++j)
                if(GETQTNODES(graph)[leaf].children[i + j*2] != -1)
                    DrawTree(graph,GETQTNODES(graph)[leaf].children[i+j*2]
                             ,(Rectangle){box.x + box.width/2.f*(float)i,box.y + box.height/2.f*(float)j,box.width/2,box.height/2});
    }
}

void DrawGraph(Graph* graph)
{
    if(!graph)
        return;
    edgeID curEdge;
    for (int i = 0; i < graph->nodes.filled; ++i) {
        if(!GETNODES(graph)[i].state)
            continue;
        DrawCircleV(GETNODES(graph)[i].pos, DOTRADIUS, BLACK);
        curEdge = GETNODES(graph)[i].edges;
        while(curEdge != EOEDGELIST)
        {
            if((GETEDGES(graph)[curEdge].node>i)&&(GETEDGES(graph)[curEdge].state&1)&&
                    (GETNODES(graph)[GETEDGES(graph)[curEdge].node].state&1))
                DrawLineV(GETNODES(graph)[i].pos,GETNODES(graph)[GETEDGES(graph)[curEdge].node].pos,BLACK);
            curEdge = GETEDGES(graph)[curEdge].nextEdge;
        }
    }
}


#define MINN(X,Y) ((X)>(Y) ? (Y) : (X))
#define ABS(X) ((X)>=0 ? (X) : (-(X)))
Rectangle TrySelect()
{
    static char isDragging = 0;
    static Vector2 dragStart = {0,0};
    if(IsMouseButtonDown(0)){
        if(isDragging)
            DrawRectangle(MINN(dragStart.x,GetMouseX()),MINN(dragStart.y,GetMouseY())
                    ,ABS(dragStart.x-GetMouseX()),ABS(dragStart.y-GetMouseY()), BLUE);
        else{
            isDragging = 1;
            dragStart.x = GetMouseX();
            dragStart.y = GetMouseY();
        }
    }
    else if(isDragging){
        isDragging = 0;
        return (Rectangle){MINN(dragStart.x,GetMouseX()),MINN(dragStart.y,GetMouseY())
                ,ABS(dragStart.x-GetMouseX()),ABS(dragStart.y-GetMouseY())};
    }
    return (Rectangle){0,0,0,0};
}

void EditVertices(Graph* graph)
{
    if(IsMouseButtonPressed(0))
        PlaceNewNode(graph, GetMousePosition());
    else if(IsMouseButtonPressed(1))
        DeleteNode(graph, FindNodeByPosition(graph,GetMousePosition()));
}

void EditEdges(Graph *graph)
{
    static char isDragging = 0;
    static nodeID dragStart = -1;
    if(IsMouseButtonDown(0)){
        if(isDragging)
            DrawLineV(GetMousePosition(), GETNODES(graph)[dragStart].pos,BLUE);
        else{
            dragStart = FindNodeByPosition(graph,GetMousePosition());
            if(dragStart != -1)
                isDragging = 1;
        }
    }
    else if(isDragging){
        isDragging = 0;
        nodeID dragEnd = FindNodeByPosition(graph, GetMousePosition());
        if(dragEnd != -1)
            FlipNodesConnection(graph, dragStart, dragEnd);
    }
}

void UpdateDrawGraphWindow(Graph* graph, int* focus)
{
    DrawGraph(graph);
    Rectangle bounds = {0,TBHEIGHT,GetScreenWidth()-IWWIDTH,GetScreenHeight()-TBHEIGHT};
    DrawTree(graph,0,(Rectangle){0,0,1200,700});
    if(!CheckCollisionPointRec(GetMousePosition(),bounds))
        return;
    if(*focus)
        switch(graphEditMode){

        case GEMMoveCamera:
            break;
        case GEMMoveVertices:
            break;
        case GEMEditVertices:
            EditVertices(graph);
            break;
        case GEMEditEdges:
            EditEdges(graph);
            break;
    }
}
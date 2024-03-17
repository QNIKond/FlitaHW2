#include "GraphWindow.h"
#include "raylib.h"
#include "Primitives/Layout.h"

GraphEditMode graphEditMode = GEMMoveCamera;

GraphEditMode *GetGraphEditMode()
{
    return &graphEditMode;
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
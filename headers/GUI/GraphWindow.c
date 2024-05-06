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

Vector2 cameraPos = {0,0};
float zoom = 1;

Vector2 GetAbs(Vector2 point){
    return (Vector2){((point.x-cameraPos.x) - GetScreenWidth()/2.)*zoom + GetScreenWidth()/2.,
                     ((point.y-cameraPos.y) - GetScreenHeight()/2.)*zoom + GetScreenHeight()/2.};
}

Rectangle GetAbsRect(Rectangle rect){
    Vector2 newPos = GetAbs((Vector2) {rect.x, rect.y});
    Vector2 newSize = GetAbs((Vector2) {rect.width + rect.x, rect.height + rect.y});
    return  (Rectangle){newPos.x,newPos.y, newSize.x-newPos.x,newSize.y-newPos.y};
}

Vector2 GetRel(Vector2 point){
    return (Vector2){(point.x - GetScreenWidth()/2.)/zoom + GetScreenWidth()/2. + cameraPos.x,
                     (point.y - GetScreenHeight()/2.)/zoom + GetScreenHeight()/2. + cameraPos.y};
}

//#define ROUND(X) ((int)X + ((int)(X*10)%10>=5))
#define ROUND(X) X
void DrawTree(Graph* graph, qtID leaf,Rectangle box)
{
    if(!graph->qtree.filled)
        return;
    Color color;
    if(GETQTNODES(graph)[leaf].isLeaf && (GETQTNODES(graph)[leaf].children[0] != -1))
        color = RED;
    else
        color = GREEN;
    if(IsKeyDown(KEY_T))
        DrawRectangleLinesEx(GetAbsRect((Rectangle) {ROUND(box.x), ROUND(box.y), ROUND(box.width), ROUND(box.height)}), 1, color);
    if(IsKeyDown(KEY_O))
    DrawCircleV(GetAbs(GETQTNODES(graph)[leaf].massCenter), GETQTNODES(graph)[leaf].mass, RED);
    if(!GETQTNODES(graph)[leaf].isLeaf)
    {
        for(int i = 0; i < 2; ++i)
            for(int j = 0; j < 2; ++j)
                if(GETQTNODES(graph)[leaf].children[i + j*2] != -1)
                    DrawTree(graph,GETQTNODES(graph)[leaf].children[i+j*2]
                             ,(Rectangle){box.x + box.width/2.f*(float)i,box.y + box.height/2.f*(float)j,box.width/2,box.height/2});
    }
}

void DrawGraph(Graph* graph, GraphConfig *gc)
{
    if(!graph)
        return;
    edgeID curEdge;
    for (int i = 0; i < graph->nodes.filled; ++i) {
        if(!GETNODES(graph)[i].state)
            continue;

        curEdge = GETNODES(graph)[i].edges;
        while(curEdge != EOEDGELIST)
        {

            if((GETEDGES(graph)[curEdge].node>i)&&(GETEDGES(graph)[curEdge].state&1)&&
                    (GETNODES(graph)[GETEDGES(graph)[curEdge].node].state&1)) {
                int weight = (GETEDGES(graph)[curEdge].weight-1) * 40;
                if(weight > 255)
                    weight = 255;
                /*DrawLineV(GetAbs(GETNODES(graph)[i].pos),
                          GetAbs(GETNODES(graph)[GETEDGES(graph)[curEdge].node].pos),
                          (Color){weight,0,0,255});*/
                if((GETEDGES(graph)[curEdge].colValue > 0) && !IsKeyDown(KEY_Z))
                    GETEDGES(graph)[curEdge].colValue -= 0.01;
                if(GETEDGES(graph)[curEdge].colValue < 0)
                    GETEDGES(graph)[curEdge].colValue = 0;
                float th = GETEDGES(graph)[curEdge].colValue;
                if(th)
                    DrawLineEx(GetAbs(GETNODES(graph)[i].pos),
                               GetAbs(GETNODES(graph)[GETEDGES(graph)[curEdge].node].pos),4*th*th+1, ColorFromHSV(GETEDGES(graph)[curEdge].hue,1,GETEDGES(graph)[curEdge].colValue));
                else if((!gc->showEdgeWeights) || (GETEDGES(graph)[curEdge].weight == 1))
                    DrawLineV(GetAbs(GETNODES(graph)[i].pos),
                              GetAbs(GETNODES(graph)[GETEDGES(graph)[curEdge].node].pos), ColorFromHSV(GETEDGES(graph)[curEdge].hue,1,GETEDGES(graph)[curEdge].colValue));
                else
                    DrawLineEx(GetAbs(GETNODES(graph)[i].pos),
                            GetAbs(GETNODES(graph)[GETEDGES(graph)[curEdge].node].pos),
                            GETEDGES(graph)[curEdge].weight, BLACK);
            }
            curEdge = GETEDGES(graph)[curEdge].nextEdge;
        }
        if(gc->showDots) {
            int weight = (GETNODES(graph)[i].weight-1) * 200;
            if(weight > 255)
                weight = 255;
            DrawCircleV(GetAbs(GETNODES(graph)[i].pos), DOTRADIUS, (Color){weight,0,0,255});
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

void UpdateCameraPosition(){
    static int isDragging = 0;
    static Vector2 dragStart = {0,0};
    static Vector2 cameraStart = {0,0};
    Vector2 mousePos = GetMousePosition();
    if(IsMouseButtonDown(0)){
        if(!isDragging){
            isDragging = 1;
            dragStart = mousePos;
            cameraStart = cameraPos;
        }
        cameraPos = (Vector2){cameraStart.x-(mousePos.x-dragStart.x)/zoom,
                              cameraStart.y-(mousePos.y-dragStart.y)/zoom};
    }
    else {
        isDragging = 0;
    }
    if(GetMouseWheelMove()>0)
        zoom *= 1.1;
    else if(GetMouseWheelMove()<0)
        zoom /= 1.1;
}

void EditVertices(Graph* graph, GraphConfig *gc)
{
    if(IsMouseButtonPressed(0)||IsMouseButtonDown(1)) {
        if(FindNodeByPosition(graph,GetRel(GetMousePosition()),10/zoom) != -1)
            graphEditMode = GEMEditEdges;
        else if(IsMouseButtonPressed(0)) {
            PlaceNewNode(graph, GetRel(GetMousePosition()));
            ++gc->components;
        }
    }
        //DeleteNode(graph, FindNodeByPosition(graph, GetRel(GetMousePosition()),10/zoom));
}

void EditEdges(Graph *graph,GraphConfig *gc)
{
    static char isDragging = 0;
    static nodeID dragNext = -1;
    static nodeID dragStart = -1;
    static int isDeleting = 0;
    if(IsMouseButtonDown(0)||IsMouseButtonDown(1)){
        if(isDragging) {
            DrawLineV(GetMousePosition(), GetAbs(GETNODES(graph)[dragNext].pos), BLUE);
            nodeID dragEnd = FindNodeByPosition(graph, GetRel(GetMousePosition()),10/zoom);
            if((dragEnd != -1) && (dragEnd != dragNext)) {
                if(IsMouseButtonDown(0))
                    gc->components += CreateNodeConnection(graph, dragNext, dragEnd) - 1;
                else{
                    /*if((dragStart == dragEnd) && (dragStart != -1)) {
                        DeleteNode(graph, dragNext);
                        isDragging = 0;
                    }*/

                    gc->components -= DisconnectNodes(graph, dragNext, dragEnd) - 1;
                    isDeleting = 0;
                }
                dragNext = dragEnd;
            }
        }
        else{
            dragNext = FindNodeByPosition(graph, GetRel(GetMousePosition()),10/zoom);
            dragStart = dragNext;
            if(dragNext != -1)
                isDragging = 1;
            if(IsMouseButtonDown(1))
                isDeleting = 1;
            else
                isDeleting = 0;
        }
    }
    else if(isDragging){
        isDragging = 0;
        if(isDeleting && (dragStart == dragNext) && (dragStart != -1)) {
            gc->components += DeleteNode(graph, FindNodeByPosition(graph, GetRel(GetMousePosition()), 10 / zoom));
        }
    }
    else{
        graphEditMode = GEMEditVertices;
    }
}

void MoveVertices(Graph *graph){
    static char isDragging = 0;
    static nodeID dragNode = -1;
    if(IsMouseButtonDown(0)){
        if(dragNode == -1)
            dragNode = FindNodeByPosition(graph, GetRel(GetMousePosition()),10/zoom);
        else
            GETNODES(graph)[dragNode].pos = GetRel(GetMousePosition());
    }
    else{
        dragNode = -1;
    }
}

void UpdateDrawGraphWindow(Graph* graph,GraphConfig *gc,int* focus)
{
    Rectangle bounds = {0,TBHEIGHT,GetScreenWidth()-IWWIDTH,GetScreenHeight()-TBHEIGHT};
    DrawRectangleRec(GetAbsRect(gc->bounds), RAYWHITE);
    DrawGraph(graph, gc);
    if(IsKeyDown(KEY_O) || IsKeyDown(KEY_T) )
        DrawTree(graph,0,gc->bounds);
    if(!CheckCollisionPointRec(GetMousePosition(),bounds))
        return;
    if(*focus)
        switch(graphEditMode){

        case GEMMoveCamera:
            UpdateCameraPosition();
            break;
        case GEMMoveVertices:
            MoveVertices(graph);
            break;
        case GEMEditVertices:
            EditVertices(graph, gc);
            break;
        case GEMEditEdges:
            EditEdges(graph,gc);
            break;
    }
}
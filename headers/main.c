#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../raygui.h"
#include "GUI/ToolBar.h"
#include "GUI/InfoWindow.h"
#include "GUI/GraphWindow.h"
#include "GUI/Keybinds.h"
#include "System/FileManager.h"

#include "Simulation/GraphSolver.h"
#include "Simulation/GraphCoarser.h"

#include "../robotofont.h"

int screenWidth = 1200;
int screenHeight = 700;

Arena graphs;
Graph graph;
Graph *curGraph;
GraphConfig gc;

void UpdateDrawFrame(void);
int main()
{
    InitWindow(screenWidth, screenHeight, "Test1");
    SetTargetFPS(60);
    SetExitKey(-1);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    GuiSetStyle(0,2,0x000000ff);
    GuiSetStyle(0, 16, 0x00000012);


    Font fnt = LoadFont_Robotofont();
    //Font fnt = LoadFontEx("Fonts/Roboto-Regular.ttf",18,0,0);
    GuiSetFont(fnt);
    //ExportFontAsCode(fnt,"../robotofont.h");
    InitializeToolBar();
    InitializeInfoWindow();
    InitializeGraph(&graph);
    curGraph = &graph;
    InitializeGraphConfig(&gc);
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
    CloseWindow();
    DestroyGraph(&graph);
    return 0;
}

int focus = 1;
void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    if(IsKeyPressed(KEY_A) && (curGraph->finerGraph)){
        curGraph = curGraph->finerGraph;
        RefineGraph(curGraph);
        //ShuffleNodes(curGraph,gc.bounds);
    }
    if(IsKeyPressed(KEY_S)){
        if(curGraph->finerGraph)
            curGraph = curGraph->finerGraph;
        //ShuffleNodes(curGraph,gc.bounds);
    }
    if(IsKeyPressed(KEY_D)){
        CoarseEC(curGraph);
        curGraph = curGraph->coarserGraph;
        //ShuffleNodes(curGraph,gc.bounds);
    }
    if(IsKeyPressed(KEY_F)){
        CoarseMIVS(curGraph);
        curGraph = curGraph->coarserGraph;
        //ShuffleNodes(curGraph,gc.bounds);
    }
    if(IsKeyPressed(KEY_M)){
        if(OpenMtx(&graph,&gc))
            curGraph = &graph;
    }
    if(IsKeyPressed(KEY_Q))
        ShuffleNodes(curGraph,gc.bounds);
    SolveGraph(curGraph, &gc);
    UpdateDrawGraphWindow(curGraph,&gc, &focus);
    UpdateDrawInfoWindow(&focus,curGraph, &gc);
    focus = 1;
    UpdateDrawToolBar(&focus);
    CheckKeyBinds(&gc);
    EndDrawing();
}
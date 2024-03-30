#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../raygui.h"
#include "SafeMemoryAllocator.h"
#include "GUI/ToolBar.h"
#include "GUI/InfoWindow.h"
#include "GUI/GraphWindow.h"
#include "GUI/Keybinds.h"
#include "System/FileManager.h"

#include "Simulation/GraphSolver.h"

#include "../robotofont.h"

int screenWidth = 1200;
int screenHeight = 700;

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
    curGraph = GetDefaultGraph();
    SetNewGraphToSolve(curGraph);
    InitializeGraphConfig(&gc);

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CloseWindow();
    DestroyGraph(curGraph);
    FreeAll();
    return 0;
}

int focus = 1;
void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    if(IsKeyPressed(KEY_Y)){
        Graph* g = OpenMtx(&gc);
        if(g)
            curGraph = g;
    }
    if(IsKeyPressed(KEY_Q))
        ShuffleNodes(curGraph,gc.bounds);
    SolveGraph(&gc);
    UpdateDrawGraphWindow(curGraph,&gc, &focus);
    UpdateDrawInfoWindow(&focus,curGraph, &gc);
    focus = 1;
    UpdateDrawToolBar(&focus);
    CheckKeyBinds(&gc);
    EndDrawing();
}
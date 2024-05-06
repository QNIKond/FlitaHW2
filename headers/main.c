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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Test1");
    SetTargetFPS(60);
    SetExitKey(-1);
    //SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
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
unsigned int k1 = 1;
unsigned int k2 = 1;
void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
    if(gc.isRunning)
        for(int i = 0; i <gc.perCycleIterations; ++i)
            SolveGraph(curGraph, &gc);
    UpdateDrawGraphWindow(curGraph,&gc, &focus);
    UpdateDrawInfoWindow(&focus,curGraph, &gc);
    focus = 1;
    UpdateDrawToolBar(&focus,&graph,&gc,&curGraph);
    CheckKeyBinds(&gc);
   /* DrawCircle(100,100,40, ColorFromHSV(50,1,(k1)));
    DrawCircle(100,100,20, ColorFromHSV(0,1,(k2)));*/
    EndDrawing();
}
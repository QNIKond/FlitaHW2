#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "headers/SafeMemoryAllocator.h"
#include "headers/GUI/ToolBar.h"
#include "headers/GUI/InfoWindow.h"
#include "headers/GUI/GraphWindow.h"

#include "headers/ArenaAllocator.h"

#include "robotofont.h"

int screenWidth = 1200;
int screenHeight = 700;

Graph graph;

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
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CloseWindow();
    DestroyGraph(&graph);
    FreeAll();
    return 0;
}


char* msg = "asdfasdf";
int focus = 1;
void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    UpdateDrawGraphWindow(&graph, &focus);
    UpdateDrawInfoWindow(&focus);
    focus = 1;
    UpdateDrawToolBar(&focus);

    EndDrawing();
}
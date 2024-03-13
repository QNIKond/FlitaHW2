#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "headers/SafeMemoryAllocator.h"
#include "headers/GUI/ToolBar.h"
#include "headers/GUI/InfoWindow.h"

#include "robotofont.h"

int screenWidth = 1200;
int screenHeight = 700;

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
    //Font fnt = LoadFont("Fonts/Roboto-Regular.ttf");
    GuiSetFont(fnt);
    ExportFontAsCode(fnt,"robotofont.h");
    InitializeToolBar();
    InitializeInfoWindow();

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CloseWindow();
    FreeAll();
    return 0;
}


char* msg = "asdfasdf";
void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
    UpdateDrawToolBar();
    UpdateDrawInfoWindow();
    EndDrawing();
}
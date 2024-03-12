#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "headers/SafeMemoryAllocator.h"
#include "headers/GUI/ToolBar.h"
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
    AssembleToolBar();

    while (!WindowShouldClose())
    {

        UpdateDrawFrame();
    }

    CloseWindow();
    FreeAll();
    return 0;
}



void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    UpdateDrawToolBar();
    EndDrawing();
}
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "headers/SafeMemoryAllocator.h"
#include "headers/GUI/Button.h"
int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(void);
char s[100];
int lastPressed = 0;
int curPressed = 0;
int main()
{
    InitWindow(screenWidth, screenHeight, "Test1");
    SetTargetFPS(60);
    SetExitKey(-1);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    int *a = 0;
    a = SMalloc(sizeof (int));
    *a = 1;
    int *b = SCalloc(4,sizeof(int));
    b[3] = 12;
    FreeAll();
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CloseWindow();
    return 0;
}



void UpdateDrawFrame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
}
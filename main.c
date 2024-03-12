#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "malloc.h"
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

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    CloseWindow();
    return 0;
}

void IntToStr(char* str, int n)
{
    char inv[100];
    int i = 0;
    do
    {
        inv[i++] = (char)(n%10+48);
        n /= 10;
    }
    while(n);
    for (int j = 0; j < i; ++j)
        str[j] = inv[i - 1 - j];
    str[i] = 0;
}

int active = 0;
int edit = 0;
Rectangle rect = {0,0,80,20};

void t1()
{
    GuiDrawText("active",(Rectangle){100,100,100,100},0,BLACK);
}
void t2()
{
    GuiDrawText("active",(Rectangle){100,100,100,100},0,BLACK);
}
void t3()
{
    GuiDrawText("active",(Rectangle){100,100,100,100},0,BLACK);
}

void UpdateDrawFrame(void)
{

    BeginDrawing();
    ClearBackground(RAYWHITE);
    if(GuiSpinner(rect,"ttt",&active,0,100,edit))
        edit = 1 - edit;
    EndDrawing();
}
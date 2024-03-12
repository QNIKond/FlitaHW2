#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "headers/SafeMemoryAllocator.h"
#include "headers/GUI/Button.h"
#include "headers/GUI/DropDownBox.h"
#include "headers/GUI/Spinner.h"

int screenWidth = 800;
int screenHeight = 450;

Rectangle rect = {0,0,50,50};
void ll(){rect.x-=10;}
void rr(int a){rect.x+=a;}
void dd(){rect.y-=10;}
void uu(){rect.y+=10;}

void UpdateDrawFrame(void);
Spinner *b1;
int main()
{
    InitWindow(screenWidth, screenHeight, "Test1");
    SetTargetFPS(60);
    SetExitKey(-1);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    b1 = CreateSpinner((Spinner){{0,0,100,40},"TTtt",10,0,100,rr});

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
    UpdateDrawSpinner(b1);
    GuiDrawRectangle(rect,1,BLACK,GRAY);
    EndDrawing();
}
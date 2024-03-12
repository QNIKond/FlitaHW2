#pragma once
#include "raylib.h"
typedef struct BUTTON
{
    Rectangle position;
    const char* text;
    void(*OnAction)();
} Button;

Button* CreateButton(Button button);

void UpdateDrawButton(Button* btn);
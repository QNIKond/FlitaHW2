#pragma once
#include "raylib.h"

typedef struct DROPDOWNBOX
{
    Rectangle position;
    const char* text;
    int size;
    void(**OnAction)();
} DropDownBox;

//DropDownBox *CreateDropDownBox(Rectangle position,const char* text, int size, void(*actions)());
DropDownBox *CreateDropDownBox(DropDownBox dropDownBox);

void UpdateDrawDropDownBox();

void FreeDropDownBox(DropDownBox* dropDownBox);
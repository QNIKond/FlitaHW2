#pragma once
#include "raylib.h"

typedef struct DROPDOWNBOX
{
    Rectangle position;
    const char* text;
    int size;
    void(**OnAction)();

    char isInEditMode;
} DropDownBox;

DropDownBox *CreateDropDownBox(DropDownBox dropDownBox);

void UpdateDrawDropDownBox(DropDownBox* drb);
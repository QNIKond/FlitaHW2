#pragma once
#include "raylib.h"

typedef struct SPINNER
{
    Rectangle position;
    const char* text;
    int value;
    int minValue;
    int maxValue;
    void(*OnAction)(int);

    char isInEditMode;
    char isApplyed;
} Spinner;

Spinner *CreateSpinner(Spinner dropDownBox);

void UpdateDrawSpinner(Spinner* sp);
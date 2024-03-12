#pragma once
#include "raylib.h"

Rectangle GetScaledPosition(Rectangle r)
{
    return (Rectangle){r.x/100*GetScreenWidth(),r.y/100*GetScreenHeight(),
                       r.width/100*GetScreenWidth(),r.height/100*GetScreenHeight()};
}
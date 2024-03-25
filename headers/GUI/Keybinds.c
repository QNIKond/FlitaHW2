#include "Keybinds.h"
#include "raylib.h"
#include "GraphWindow.h"
void CheckKeyBinds(GraphConfig *gc){
    if(IsKeyDown(KEY_SPACE))
        *GetGraphEditMode() = GEMMoveCamera;
    else if(*GetGraphEditMode() == GEMMoveCamera)
        *GetGraphEditMode() = GEMEditVertices;
    if(IsKeyPressed(KEY_S))
        gc->globalIterations = 1;
    if(IsKeyPressed(KEY_R))
        gc->isRunning = !gc->isRunning;
}
#include "InfoWindow.h"
//#define RAYGUI_IMPLEMENTATION
#include "../../raygui.h"
#include "raylib.h"
#include "Layout.h"
#include "stdlib.h"
#include "../Simulation/QuadTree.h"

#include "stdio.h"

enum {
    IWMinimized,
    IWInfo,
    IWSettings,
    IWHelp
} IWState = IWInfo;

#define SETTINGSCOUNT 3
typedef struct{
    const char* text;
    double minValue;
    double maxValue;
    int isInt;
    char editValue[20];
    int isActive;
} Setting;

Setting settings[SETTINGSCOUNT];

void InitializeInfoWindow()
{
    for(int i = 0; i < SETTINGSCOUNT; ++i)
        settings[i] = (Setting){0, 0, 0, 0,"", 0};
    settings[0] = (Setting){"Bounds width", 30, 500, 0,"", 0};
    settings[1] = (Setting){"Bounds height", 100, 2000, 0,"", 0};
    settings[2] = (Setting){"Root state", 0, 10, 1,"", 0};
}

void DrawInfoLine(Rectangle *bounds, const char* text, double value,int acc){
    char valstr[30];
    //itoa(value,valstr,10);
    sprintf(valstr,"%.*lf",acc,value);

    GuiLabel(*bounds,TextJoin((const char*[]){text,valstr},2,""));
    bounds->y += bounds->height;
}

void UpdateDrawInfoPanel(Rectangle rect, Graph *graph, GraphConfig *gc){
    Rectangle textBounds = {rect.x+10,rect.y+18,rect.width,18};
    DrawInfoLine(&textBounds,"Filled nodes count:    ",graph->nodes.filled,0);
    DrawInfoLine(&textBounds,"Filled edges count:    ",graph->edges.filled,0);
    DrawInfoLine(&textBounds,"root:    ", GETQTNODES(graph)[0].mass,0);
    DrawInfoLine(&textBounds,"root:    ", GETQTNODES(graph)[0].massCenter.x,0);
}

void UpdateDrawSettingsLine(Rectangle *bounds,Setting *setting,void *value){
    GuiLabel(*bounds,setting->text);
    Rectangle numBounds = {bounds->x+bounds->width/2,bounds->y, bounds->width,bounds->height};
    int changed = GuiTextBox(numBounds,setting->editValue,20,setting->isActive);
    if(changed)
        setting->isActive = !setting->isActive;
    if(!setting->isActive){
        if(changed){
            float result = setting->minValue - 1;
            sscanf((const char *) setting->editValue, "%f", &result);
            if ((result >= setting->minValue) && (result <= setting->maxValue)) {
                if (setting->isInt)
                    *(int *) value = result;
                else
                    *(float *) value = result;
            }
        }
        else{
            if (setting->isInt)
                sprintf_s(setting->editValue,20,"%d",*(int*)value);
            else
                sprintf_s(setting->editValue,20,"%f",*(float *)value);
        }
    }
    bounds->y += bounds->height;
}

void UpdateDrawSettingsPanel(Rectangle rect, Graph *graph, GraphConfig *gc){
    Rectangle textBounds = {rect.x+10,rect.y+18,rect.width,18};
    UpdateDrawSettingsLine(&textBounds,&settings[0],&gc->bounds.width);
    UpdateDrawSettingsLine(&textBounds,&settings[1],&gc->bounds.height);
    UpdateDrawSettingsLine(&textBounds,&settings[2], &GETNODES(graph)[0].state);
}

void UpdateDrawHelpPanel(Rectangle rect)
{
    /*GuiTextBox(rect,"Press Alt+F4 to exit.",20,1);*/
    DrawText("Press Alt+F4 to exit.",rect.x+10,rect.y+10,20,BLACK);
}

void UpdateDrawIWStatePanel(int *focus)
{
    Rectangle rect = {GetScreenWidth()-IWWIDTH,TBHEIGHT,IWWIDTH/4,TBHEIGHT};
    DrawRectangle(GetScreenWidth()-IWWIDTH,TBHEIGHT,IWWIDTH,TBHEIGHT,DARKGREEN);
    int state = IWState;
    GuiToggleGroup(rect,">;Info;Settings;Help",&state);
    if(*focus)
        IWState = state;
}

void UpdateDrawInfoWindow(int *focus, Graph *graph, GraphConfig *gc){
    UpdateDrawIWStatePanel(focus);
    Rectangle rect = {GetScreenWidth()-IWWIDTH,TBHEIGHT*2,IWWIDTH,GetScreenHeight()-TBHEIGHT*2};
    if(IWState!=IWMinimized)
        DrawRectangleRec(rect,WHITE);
    switch (IWState) {
        case IWMinimized:
            break;
        case IWInfo:
            UpdateDrawInfoPanel(rect, graph, gc);
            break;
        case IWSettings:
            UpdateDrawSettingsPanel(rect, graph, gc);
            break;
        case IWHelp:
            UpdateDrawHelpPanel(rect);
            break;
    }
}
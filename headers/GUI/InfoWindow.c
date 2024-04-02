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

#define SETTINGSCOUNT 30
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
    settings[0] = (Setting){"Bounds width", 10, 5000, 0,"", 0};
    settings[1] = (Setting){"Bounds height", 10, 5000, 0,"", 0};
    settings[2] = (Setting){"Edge resize constant", 0.00001, 1000, 0,"", 0};
    settings[3] = (Setting){"Repulsive force strength", 0.00001, 1000, 0,"", 0};
    settings[4] = (Setting){"Low heat", 0.00001, 1000, 0,"", 0};
    settings[5] = (Setting){"Super node ratio", 0.00001, 1000, 0,"", 0};
    settings[6] = (Setting){"Max tree depth", 1, 100, 1,"", 0};
    settings[7] = (Setting){"Use qt optimization", 0, 1, 1,"", 0};
    settings[8] = (Setting){"Show dots", 0, 1, 1,"", 0};
    settings[9] = (Setting){"Show weights", 0, 1, 1,"", 0};
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
    DrawInfoLine(&textBounds,"Filled nodes count:    ", GETQTNODES(graph)[0].mass,0);
    DrawInfoLine(&textBounds,"Nodes:    ",gc->verticesCount,0);
    DrawInfoLine(&textBounds,"Edges:    ",gc->edgesCount,0);
    DrawInfoLine(&textBounds,"Desired length:    ",gc->K,3);
    DrawInfoLine(&textBounds,"Edge:    ", GETEDGES(graph)[0].state,0);

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
    UpdateDrawSettingsLine(&textBounds,&settings[2],&gc->edgeResizeConst);
    UpdateDrawSettingsLine(&textBounds,&settings[3],&gc->C);
    UpdateDrawSettingsLine(&textBounds,&settings[4],&gc->heat);
    UpdateDrawSettingsLine(&textBounds,&settings[5],&gc->superNodeRatio);
    UpdateDrawSettingsLine(&textBounds,&settings[6],&gc->maxTreeDepth);
    UpdateDrawSettingsLine(&textBounds,&settings[7],&gc->useQTOptimization);
    UpdateDrawSettingsLine(&textBounds,&settings[8],&gc->showDots);
    UpdateDrawSettingsLine(&textBounds,&settings[9],&gc->showEdgeWeights);
}

void UpdateDrawHelpPanel(Rectangle rect)
{

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
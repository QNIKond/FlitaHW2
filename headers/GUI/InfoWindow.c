#include "InfoWindow.h"
#include "../../raygui.h"
#include "Primitives/Layout.h"

enum {
    IWMinimized,
    IWInfo,
    IWSettings,
    IWHelp
} IWState = IWInfo;

void InitializeInfoWindow()
{

}

void UpdateDrawInfoPanel(){

}

void UpdateDrawSettingsPanel(){

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

void UpdateDrawInfoWindow(int *focus){
    UpdateDrawIWStatePanel(focus);
    Rectangle rect = {GetScreenWidth()-IWWIDTH,TBHEIGHT*2,IWWIDTH,GetScreenHeight()-TBHEIGHT*2};
    if(IWState!=IWMinimized)
        DrawRectangleRec(rect,WHITE);
    switch (IWState) {
        case IWMinimized:
            break;
        case IWInfo:
            UpdateDrawInfoPanel();
            break;
        case IWSettings:
            UpdateDrawSettingsPanel();
            break;
        case IWHelp:
            UpdateDrawHelpPanel(rect);
            break;
    }
}
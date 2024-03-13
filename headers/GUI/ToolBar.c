#include "ToolBar.h"
#include "Primitives/Button.h"
#include "Primitives/DropDownBox.h"
#include "../../raygui.h"
#include "Primitives/Layout.h"
#define WIDTHOFDROPDOWN 100
#define WIDTHOFBUTTON 50
#define DIVIDERWIDTH 30

Button *buttons[4];
DropDownBox *dropDowns[3];

int t = 18;

void tplus()
{
    t+=1;
    GuiSetStyle(0, 16, t);
}
void tminus()
{
    t-=1;
    GuiSetStyle(0, 16, t);
}

void teq()
{
    GuiSetStyle(0, 16, 18);
}

void InitializeToolBar()
{
    int head = 0;
    dropDowns[0] = CreateDropDownBox((DropDownBox){{head,0,WIDTHOFDROPDOWN,TBHEIGHT}
    ,"File; New; Save; Save as; Load; Export",0,0});
    head += WIDTHOFDROPDOWN;
    dropDowns[1] = CreateDropDownBox((DropDownBox){{head,0,WIDTHOFDROPDOWN,TBHEIGHT}
    ,"Edit; Move; Create vertices; Create edges",0,0});
    head += WIDTHOFDROPDOWN;
    dropDowns[2] = CreateDropDownBox((DropDownBox){{head,0,WIDTHOFDROPDOWN,TBHEIGHT}
    ,"View; show dots; center camera",0,0});
    head += WIDTHOFDROPDOWN + DIVIDERWIDTH;

    buttons[0] = CreateButton((Button){{head,0,WIDTHOFBUTTON,TBHEIGHT},"Run",tplus});
    head += WIDTHOFBUTTON;
    buttons[1] = CreateButton((Button){{head,0,WIDTHOFBUTTON,TBHEIGHT},"Step",teq});
    head += WIDTHOFBUTTON;
    buttons[2] = CreateButton((Button){{head,0,WIDTHOFBUTTON,TBHEIGHT},"Stop",tminus});
    head += WIDTHOFBUTTON + DIVIDERWIDTH;
    buttons[3] = CreateButton((Button){{head,0,WIDTHOFBUTTON*2,TBHEIGHT},"Shuffle",0});
}

void UpdateDrawToolBar()
{
    DrawRectangle(0,0,GetScreenWidth(),TBHEIGHT,GRAY);
    for(int i = 0; i < 4; ++i)
        UpdateDrawButton(buttons[i]);
    for(int i = 0; i < 3; ++i)
        UpdateDrawDropDownBox(dropDowns[i]);
}
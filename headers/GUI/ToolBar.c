#include "ToolBar.h"
#include "Primitives/Button.h"
#include "Primitives/DropDownBox.h"
#include "../../raygui.h"

#define WIDTHOFDROPDOWN 100
#define WIDTHOFBUTTON 50
#define DIVIDERWIDTH 30

Button *buttons[4];
DropDownBox *dropDowns[3];

void ttt()
{
    DrawRectangle(100,100,100,100,RED);
}

void AssembleToolBar()
{
    int head = 0;
    dropDowns[0] = CreateDropDownBox((DropDownBox){{head,0,WIDTHOFDROPDOWN,25}
    ,"File; New; Save; Save as; Load; Export",0,0});
    head += WIDTHOFDROPDOWN;
    dropDowns[1] = CreateDropDownBox((DropDownBox){{head,0,WIDTHOFDROPDOWN,25}
    ,"Edit; Move; Create vertices; Create edges",0,0});
    head += WIDTHOFDROPDOWN;
    dropDowns[2] = CreateDropDownBox((DropDownBox){{head,0,WIDTHOFDROPDOWN,25}
    ,"View; show dots; center camera",0,0});
    head += WIDTHOFDROPDOWN + DIVIDERWIDTH;

    buttons[0] = CreateButton((Button){{head,0,WIDTHOFBUTTON,25},"Run",0});
    head += WIDTHOFBUTTON;
    buttons[1] = CreateButton((Button){{head,0,WIDTHOFBUTTON,25},"Step",0});
    head += WIDTHOFBUTTON;
    buttons[2] = CreateButton((Button){{head,0,WIDTHOFBUTTON,25},"Bake",0});
    head += WIDTHOFBUTTON + DIVIDERWIDTH;
    buttons[3] = CreateButton((Button){{head,0,WIDTHOFBUTTON,25},"Shuffle",0});
}

void UpdateDrawToolBar()
{
    for(int i = 0; i < 4; ++i)
        UpdateDrawButton(buttons[i]);
    for(int i = 0; i < 3; ++i)
        UpdateDrawDropDownBox(dropDowns[i]);
}
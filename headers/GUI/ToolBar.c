#include "ToolBar.h"
#include "../../raygui.h"
#include "GraphWindow.h"
#include "Primitives/Layout.h"
#define WIDTHOFDROPDOWN 100
#define WIDTHOFEDIT 150
#define WIDTHOFBUTTON 50
#define DIVIDERWIDTH 30

GraphEditMode *graphEditRef;

struct Menu{
    const char* text;
    int selected;
    int lastState;
    int isActive;
    int isSwitchable;
}fileMenu,viewMenu,editMenu;

void InitializeToolBar()
{
    graphEditRef = GetGraphEditMode();
    fileMenu = (struct Menu){"File;Save;Load;Import",0,0,0,0};
    viewMenu = (struct Menu){"View;Show/Hide dots; center camera",0,0,0,0};
    editMenu = (struct Menu){"Move;Grab;Add/delete vertices;Add/delete Edges",0,0,0,1};
}

int UpdatedrawMenu(Rectangle pos,struct Menu *menu)
{
    if(!menu->isSwitchable)
        menu->selected = 0;
    if(GuiDropdownBox(pos,menu->text,&menu->selected,menu->isActive))
        menu->isActive = !menu->isActive;
    if((!menu->isSwitchable && menu->selected) || (menu->isSwitchable && (menu->selected!=menu->lastState)))
    {
        menu->lastState = menu->selected;
        return menu->selected;
    }
    return -1;
}

void UpdateDrawToolBar(int* focus)
{
    DrawRectangle(0,0,GetScreenWidth(),TBHEIGHT,GRAY);
    if(editMenu.isActive || fileMenu.isActive || viewMenu.isActive)
        *focus = 0;
    switch(UpdatedrawMenu((Rectangle){0,0,WIDTHOFDROPDOWN,TBHEIGHT},&fileMenu)){

    }
    switch(UpdatedrawMenu((Rectangle){WIDTHOFDROPDOWN,0,WIDTHOFDROPDOWN,TBHEIGHT},&viewMenu)){

    }
    GuiTextBox((Rectangle){WIDTHOFDROPDOWN*2+20,0,WIDTHOFEDIT,TBHEIGHT}
    ,"Edit mode:",18,0);

/*    DrawTextEx("Edit mode:",GetScreenWidth() - WIDTHOFEDIT*2,0,18,BLACK);*/
    int edit = UpdatedrawMenu((Rectangle){WIDTHOFDROPDOWN*3,0,WIDTHOFEDIT,TBHEIGHT},&editMenu);
    if(edit != -1)
        *graphEditRef = edit;
}
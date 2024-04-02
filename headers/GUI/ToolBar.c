#include "ToolBar.h"
#include "../../raygui.h"
#include "Layout.h"
#include "../System/FileManager.h"
#include "../Simulation/GraphCoarser.h"
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
    fileMenu = (struct Menu){"File;Load adj mtx;Load edges list;Load mtx;Save png",0,0,0,0};
    //viewMenu = (struct Menu){"View;Show/Hide dots; center camera",0,0,0,0};
    //editMenu = (struct Menu){"Move;Grab;Add/delete vertices;Add/delete Edges",2,2,0,1};
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

void UpdateDrawToolBar(int* focus, Graph *graph, GraphConfig *gc, Graph **curGraph)
{
    DrawRectangle(0,0,GetScreenWidth(),TBHEIGHT,GRAY);
    if(editMenu.isActive || fileMenu.isActive || viewMenu.isActive)
        *focus = 0;
    switch(UpdatedrawMenu((Rectangle){0,0,WIDTHOFDROPDOWN,TBHEIGHT},&fileMenu)){
        case 1:
            if(OpenAdjacencyMatrix(graph,gc))
                *curGraph = graph;
            break;
        case 2:
            if(OpenEdgesList(graph,gc))
                *curGraph = graph;
            break;
        case 3:
            if(OpenMtx(graph,gc))
                *curGraph = graph;
            break;
        case 4:
            DrawToImage(*curGraph,gc);
            break;
    }
    if(GuiButton((Rectangle){200, 0, 50, TBHEIGHT},"RUN"))
        gc->isRunning = 1;
    if(GuiButton((Rectangle){250, 0, 50, TBHEIGHT},"STOP"))
        gc->isRunning = 0;
    if(GuiButton((Rectangle){300, 0, 100, TBHEIGHT},"Shuffle"))
        ShuffleNodes(*curGraph,gc->bounds);

    if(GuiButton((Rectangle){500, 0, 100, TBHEIGHT},"Refine") && ((*curGraph)->finerGraph)){
        *curGraph = (*curGraph)->finerGraph;
        RefineGraph(*curGraph);
        //ShuffleNodes(curGraph,gc.bounds);
    }
    if(GuiButton((Rectangle){600, 0, 100, TBHEIGHT},"Back")){
        if((*curGraph)->finerGraph)
            *curGraph = (*curGraph)->finerGraph;
        //ShuffleNodes(curGraph,gc.bounds);
    }
    if(GuiButton((Rectangle){700, 0, 100, TBHEIGHT},"Coarse EC")){
        CoarseEC(*curGraph);
        *curGraph = (*curGraph)->coarserGraph;
        //ShuffleNodes(curGraph,gc.bounds);
    }
    if(GuiButton((Rectangle){800, 0, 100, TBHEIGHT},"Coarce MIVS")){
        CoarseMIVS(*curGraph);
        *curGraph = (*curGraph)->coarserGraph;
        //ShuffleNodes(curGraph,gc.bounds);
    }
    /*GuiTextBox((Rectangle){WIDTHOFDROPDOWN*2+20,0,WIDTHOFEDIT,TBHEIGHT}
    ,"Edit mode:",18,0);*/

/*    DrawTextEx("Edit mode:",GetScreenWidth() - WIDTHOFEDIT*2,0,18,BLACK);*/
    /*int edit = UpdatedrawMenu((Rectangle){WIDTHOFDROPDOWN*3,0,WIDTHOFEDIT,TBHEIGHT},&editMenu);
    if(edit != -1)
        *graphEditRef = edit;*/
}
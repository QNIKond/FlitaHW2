#include "Button.h"
#include "../../SafeMemoryAllocator.h"
#include "../../../raygui.h"

Button* CreateButton(Button button)
{
    Button *new = SMalloc(sizeof(button));
    *new = button;
    return new;
}

void UpdateDrawButton(Button* btn)
{
    if(GuiButton(btn->position, btn->text) && btn->OnAction)
        btn->OnAction();
}
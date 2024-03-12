#include "DropDownBox.h"
#include "../../SafeMemoryAllocator.h"
#include "../../../raygui.h"

DropDownBox *CreateDropDownBox(DropDownBox dropDownBox)
{
    DropDownBox *new = SMalloc(sizeof(dropDownBox));
    *new = dropDownBox;
    if(dropDownBox.size) {
        void (**OnAction)() = SCalloc(dropDownBox.size, sizeof(void *));
        for (int i = 0; i < dropDownBox.size; ++i)
            OnAction[i] = dropDownBox.OnAction[i];
        new->OnAction = OnAction;
    }
    else
        new->OnAction = 0;
    new->isInEditMode = 0;
    return new;
}

void UpdateDrawDropDownBox(DropDownBox* drb)
{
    int selected = 0;
    if(GuiDropdownBox(drb->position, drb->text,&selected,drb->isInEditMode))
        drb->isInEditMode = (char)(!drb->isInEditMode);
    if(selected && drb->OnAction && drb->OnAction[selected-1])
        drb->OnAction[selected-1]();
}

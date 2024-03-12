#include "Spinner.h"
#include "../../SafeMemoryAllocator.h"
#include "../../../raygui.h"

Spinner *CreateSpinner(Spinner dropDownBox)
{
    Spinner *new = SMalloc(sizeof(dropDownBox));
    *new = dropDownBox;
    new->isInEditMode = 0;
    new->isApplyed = 1;
    return new;
}

void UpdateDrawSpinner(Spinner* sp)
{
    int newValue = sp->value;
    if(GuiSpinner(sp->position, sp->text, &newValue,sp->minValue,sp->maxValue,sp->isInEditMode))
        sp->isInEditMode = (char)(!sp->isInEditMode);
    if(newValue!=sp->value)
        sp->isApplyed = 0;
    if((!sp->isInEditMode)&&(!sp->isApplyed)) {
        sp->OnAction(newValue);
        sp->isApplyed = 1;
    }
    sp->value = newValue;
}
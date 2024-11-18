// TinyTavern


#include "ControlListEntry.h"

#include "CommonActionWidget.h"
#include "CommonTextBlock.h"
#include "InputAction.h"
#include "Components/Image.h"

void UControlListEntry::SetControl(FText ControlText, FDataTableRowHandle ControlImage)
{
	ContentTextBlock.Get()->SetText(ControlText);
	InputImage.Get()->SetInputAction(ControlImage);
}

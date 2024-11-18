// TinyTavern


#include "TutorialUI.h"
#include "CommonActionWidget.h"
#include "CommonTextBlock.h"
#include "Components/HorizontalBox.h"


void UTutorialUI::EstablishTutorialText(FText Text1, FText Text2, FText Text3, FText Text4, FText Text5)
{
	TutorialTextBlock1.Get()->SetText(Text1);
	TutorialTextBlock2.Get()->SetText(Text2);
	TutorialTextBlock3.Get()->SetText(Text3);
	TutorialTextBlock4.Get()->SetText(Text4);
	TutorialTextBlock5.Get()->SetText(Text5);
}

void UTutorialUI::EstablishInputActions(FDataTableRowHandle Input1, FDataTableRowHandle Input2, FDataTableRowHandle Input3, FDataTableRowHandle Input4)
{
	ActionWidget1.Get()->SetInputAction(Input1);
	ActionWidget2.Get()->SetInputAction(Input2);
	ActionWidget3.Get()->SetInputAction(Input3);
	ActionWidget4.Get()->SetInputAction(Input4);
}

void UTutorialUI::SetInteractVisibility(ESlateVisibility NewVisibility)
{
	InteractBox.Get()->SetVisibility(NewVisibility);
}

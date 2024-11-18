// TinyTavern


#include "NPCQuestionButton.h"

#include "ArticyFlowPlayer.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/NPCDialog.h"

void UNPCQuestionButton::SetNPCDialog(TObjectPtr<UNPCDialog> _NPCDialog)
{
	NPCDialog = _NPCDialog;
}

void UNPCQuestionButton::NativeOnClicked()
{
	Super::NativeOnClicked();

	PlayerArticyFlowPlayer.Get()->SetStartNodeById(DialogID);
	NPCParent.Get()->SetInAQuestion(true);
}

void UNPCQuestionButton::NativeOnHovered()
{
	Super::NativeOnHovered();
	QuestionText = ButtonText.Get()->GetText();
	ArrowImage.Get()->SetVisibility(ESlateVisibility::Visible);
	if(NPCDialog != nullptr)
	{
		NPCDialog.Get()->ChangeGamepadCurrentCutton(this);
	}
}

void UNPCQuestionButton::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
    ArrowImage.Get()->SetVisibility(ESlateVisibility::Hidden);
}

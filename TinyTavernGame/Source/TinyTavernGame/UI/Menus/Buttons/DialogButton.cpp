// TinyTavern


#include "DialogButton.h"

#include "ArticyFlowPlayer.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBoxSlot.h"
#include "TinyTavernGame/ArticyGenerated/Tutorial_Dialogue_1st_DraftArticyTypes.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/NPCDialog.h"

void UDialogButton::NativeConstruct()
{
	Super::NativeConstruct();
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
}

void UDialogButton::SetNPCDialog(TObjectPtr<UNPCDialog> _NPCDialog)
{
	NPCDialog = _NPCDialog;
}

void UDialogButton::AssignText(FArticyBranch _Branch, TObjectPtr<UArticyFlowPlayer> _PlayerArticyFlowPlayer, TObjectPtr<UTutorial_Dialogue_1st_DraftDialogueFragment> _ArticyTutorialDialogueFragment)
{
	ArticyTutorialDialogueFragment = nullptr;
	ArticyTutorialDialogueFragment = _ArticyTutorialDialogueFragment;
	AlreadyRead = false;
	ButtonText.Get()->SetColorAndOpacity(NormalColor);
	Branch = _Branch;
	PlayerArticyFlowPlayer = _PlayerArticyFlowPlayer;
	BranchText = ArticyTutorialDialogueFragment.Get()->MenuText;
	if(BranchText.IsEmpty())
	{
		BranchText = FText::FromString("CONTINUE");
 		SetTextOfButton(BranchText);
	}
	else
	{
 		SetTextOfButton(BranchText);
		//Check if player already interacted with this branch
        if(TTGameInstance.Get()->DialogsRead.Contains(ArticyTutorialDialogueFragment.Get()->GetTechnicalName()))
        {
        	BranchAlreadySelected();	
        }
	}
}

void UDialogButton::BranchAlreadySelected()
{
	ButtonText.Get()->SetColorAndOpacity(AlreadyReadColor);
	AlreadyRead = true;
}

void UDialogButton::NativeOnHovered()
{
	Super::NativeOnHovered();
	ButtonText.Get()->SetColorAndOpacity(HoverColor);
	ArrowImage.Get()->SetVisibility(ESlateVisibility::Visible);
	if(NPCDialog != nullptr)
	{
		NPCDialog.Get()->ChangeGamepadCurrentCutton(this);
	}
}

void UDialogButton::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	AlreadyRead ? ButtonText.Get()->SetColorAndOpacity(AlreadyReadColor) : ButtonText.Get()->SetColorAndOpacity(NormalColor);
	ArrowImage.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UDialogButton::NativeOnClicked()
{
	Super::NativeOnClicked();
	//
	NativeOnUnhovered();
	SetVisibility(ESlateVisibility::Collapsed);
	FText branchText = ArticyTutorialDialogueFragment.Get()->MenuText;
	if(!branchText.IsEmpty())
	{
		//Check if player already interacted with this branch
		if(!TTGameInstance.Get()->DialogsRead.Contains(ArticyTutorialDialogueFragment.Get()->GetTechnicalName()))
		{
			TTGameInstance.Get()->DialogsRead.Add(ArticyTutorialDialogueFragment.Get()->GetTechnicalName());
		}
	}
	//Play Branch
	PlayerArticyFlowPlayer->PlayBranch(Branch);
}

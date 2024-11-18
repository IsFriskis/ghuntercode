// TinyTavern


#include "NPCOptionButton.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "TinyTavernGame/UI/Menus/Tavern/NPCS/NPCDialog.h"

void UNPCOptionButton::NativeConstruct()
{
	Super::NativeConstruct();
	NormalColorCopy = NormalColor;
}

void UNPCOptionButton::SetNPCDialog(TObjectPtr<UNPCDialog> _NPCDialog)
{
	NPCDialog = _NPCDialog;
}

void UNPCOptionButton::ChangeNormalColor()
{
	NormalColor = UpgradeRelicNeededTextColor;
	ButtonText.Get()->SetColorAndOpacity(NormalColor);
}

void UNPCOptionButton::SetNormalColor()
{
	NormalColor = NormalColorCopy;
	ButtonText.Get()->SetColorAndOpacity(NormalColor);
}


void UNPCOptionButton::NativeOnHovered()
{
	Super::NativeOnHovered();
	ButtonText.Get()->SetColorAndOpacity(HoverColor);
	ArrowImage.Get()->SetVisibility(ESlateVisibility::Visible);
	if(NPCDialog != nullptr)
	{
		NPCDialog.Get()->ChangeGamepadCurrentCutton(this);
	}
}

void UNPCOptionButton::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	ButtonText.Get()->SetColorAndOpacity(NormalColor);
	ArrowImage.Get()->SetVisibility(ESlateVisibility::Hidden);
}

// TinyTavern


#include "TPButton.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UTPButton::NativeConstruct()
{
	Super::NativeConstruct();
	if(!Unlocked)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTPButton::SetAsUnlocked()
{
	SetVisibility(ESlateVisibility::Visible);
	Unlocked = true;
}

void UTPButton::SetAsCurrentTP()
{
	TPImage.Get()->SetBrushFromTexture(CurrentTPTexture);
	IsCurrentTP = true;
}


bool UTPButton::IsUnlocked()
{
	return Unlocked;
}

int UTPButton::GetID()
{
	return ID;
}

void UTPButton::NativeOnClicked()
{
	Super::NativeOnClicked();
	OnTpButtonClicked.Broadcast(this);
}

// TinyTavern


#include "EasterEggUI.h"

#include "Input/CommonUIInputTypes.h"
#include "TinyTavernGame/Levels/TavernLevel.h"

void UEasterEggUI::NativeConstruct()
{
	Super::NativeConstruct();
	//Get reference to level
	TavernLevel = Cast<ATavernLevel>(GetWorld()->GetLevelScriptActor());
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UEasterEggUI::BackLogic)));
}

void UEasterEggUI::NativeDestruct()
{
	Super::NativeDestruct();
	BackHandle.Unregister();
}

void UEasterEggUI::BackLogic()
{
	BackHandle.Unregister();
	
	TavernLevel.Get()->EasterEggShowAnimation(false);
}

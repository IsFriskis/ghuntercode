// TinyTavern


#include "StatueUI.h"

#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Input/CommonUIInputTypes.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Actor/Structures/Statue.h"
#include "TinyTavernGame/Actor/Resources/ResourceManager.h"
#include "TinyTavernGame/Recipes/Recipes_DataTable.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/Menus/Buttons/InventorySlot.h"

void UStatueUI::NativeConstruct()
{
	Super::NativeConstruct();
	TTGameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	TTGameInstance.Get()->StatuesPlatesDataTable->GetAllRows("",StatuesPlates);
	OfferButton.Get()->OnClicked().AddUObject(this, &UStatueUI::AfterGaliasWords);

	//Start Statue
	StatueDialogBox.Get()->SetText(BP_FormatTextForEnter());
	PlaySound(EnteringStatueSound);
	OfferButton.Get()->SetTextOfButton(FText::FromString("NEXT"));
	//Handlers
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, false, FSimpleDelegate::CreateUObject(this, &UStatueUI::ExitStatue)));
	AcceptHandle = RegisterUIActionBinding(FBindUIActionArgs(AcceptInputActionData, false, FSimpleDelegate::CreateUObject(this, &UStatueUI::AfterGaliasWords)));
}

void UStatueUI::NativeDestruct()
{
	Super::NativeDestruct();

	StatuesPlates.Empty();
	AcceptHandle.Unregister();
	BackHandle.Unregister();
	//Remove Bindings
	OfferButton.Get()->OnClicked().RemoveAll(this);
}

FReply UStatueUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(IsOnGaliasWords)
	{
		AfterGaliasWords();
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UStatueUI::SetStatue(int _StatueID, TObjectPtr<AStatue> _Statue, FText _StatueRiddle, bool _IsPlateStatue,
	FGameplayTag _WeaponToBeUnlocked)
{
	Statue = _Statue;
	IsPlateStatue = _IsPlateStatue;
	WeaponToBeUnlocked = _WeaponToBeUnlocked;
	StatueID = _StatueID;
	FText title;
	//Set Text
	for (FStatuePlates* statuePlate : StatuesPlates)
	{
		if (statuePlate->StatueID == StatueID)
		{
			StatuePlateID = statuePlate->PlateID;
			StatueRiddle = statuePlate->PlateText;
			title = statuePlate->PlateTitle;
			break;
		}
	}
	StatueRiddle = BP_FormatRiddleForEnter();
	if(!IsPlateStatue)
	{
		StatueRiddle = _StatueRiddle;
		StatueRiddle = BP_FormatRiddleForEnter();
		GaliasWords = GaliasUpgradeWords;
		StatueDialogBox.Get()->SetText(BP_FormatTextForEnter());
		StatueTitleImage.Get()->SetVisibility(ESlateVisibility::Hidden);
		StatueTitle.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		StatueTitle.Get()->SetText(title);
	}
}

void UStatueUI::AfterGaliasWords()
{
	StatueCanvasPanel.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OfferButton.Get()->OnClicked().RemoveAll(this);
	OfferButton.Get()->OnClicked().AddUObject(this, &UStatueUI::OfferPlate);
	IsOnGaliasWords = false;
	AcceptHandle.Unregister();
	AcceptHandle = RegisterUIActionBinding(FBindUIActionArgs(AcceptInputActionData, false, FSimpleDelegate::CreateUObject(this, &UStatueUI::OfferPlate)));
	StatueDialogBox.Get()->SetText(StatueRiddle);
	OfferButton.Get()->SetTextOfButton(FText::FromString("OFFER SACRIFICE"));
	if(IsPlateStatue)
	{
		if(!TTGameInstance.Get()->StatuesPlatesInInventory.Contains(StatuePlateID))
		{
			TTGameInstance.Get()->StatuesPlatesInInventory.Add(StatuesPlates[StatueID]->PlateID, false);
		}
	}
	else
	{
		OfferButton.Get()->SetTextOfButton(FText::FromString("OBTAIN GIFT"));
		StatueDialogBox.Get()->SetText(StatueRiddle);
	}
}

void UStatueUI::OfferPlate()
{
	AcceptHandle.Unregister();
	if(IsPlateStatue)
	{
		if(TTGameInstance.Get()->StatuesPlatesInInventory.Contains(StatuePlateID) && TTGameInstance.Get()->StatuesPlatesInInventory[StatuePlateID])
		{
			TTGameInstance.Get()->StatuesPlatesInInventory.Remove(StatuePlateID);
			//Achievement of opening all statues
			TTGameInstance.Get()->OpenAllStatues();
			Statue.Get()->StatueCompleted();
		}
		else
		{
			AcceptHandle = RegisterUIActionBinding(FBindUIActionArgs(AcceptInputActionData, false, FSimpleDelegate::CreateUObject(this, &UStatueUI::ExitStatue)));
			OfferButton.Get()->OnClicked().RemoveAll(this);
			OfferButton.Get()->OnClicked().AddUObject(this, &UStatueUI::ExitStatue);
			OfferButton.Get()->SetTextOfButton(FText::FromString("CLOSE"));
			StatueDialogBox.Get()->SetText(FText::FromString("You have not yet prepared the dish I demand"));
		}
	}
	else
	{
		if(CheckIfWeaponCanBeUnlocked())
		{
			Statue.Get()->StatueCompleted();
		}
		else
		{
			Statue.Get()->StatueExit();
		}
	}
}

void UStatueUI::ExitStatue()
{
	if(!IsPlateStatue)
	{
		if(CheckIfWeaponCanBeUnlocked())
		{
			Statue.Get()->StatueCompleted();
			return;
		}
	}
	Statue.Get()->StatueExit();
}

bool UStatueUI::CheckIfWeaponCanBeUnlocked()
{
	//Weapon Unlocked Statue
	if((WeaponToBeUnlocked == FGameplayTags::Get().Weapon_Range_Boil &&
		TTGameInstance.Get()->UnlockedWeapons[FGameplayTags::Get().Weapon_CanObtainBoil]))
	{
		TTGameInstance.Get()->UnlockedWeapons[WeaponToBeUnlocked] = true;
		TTGameInstance.Get()->GetSubsystem<UResourceManager>()->NotifyCanObtainBoilResource();
		//Achievement of Totem
		TTGameInstance.Get()->TotemUnlocked(WeaponToBeUnlocked);
		return true;
	}
	else if((WeaponToBeUnlocked == FGameplayTags::Get().Weapon_Range_Fry &&
		TTGameInstance.Get()->UnlockedWeapons[FGameplayTags::Get().Weapon_CanObtainFry]))
	{
		TTGameInstance.Get()->UnlockedWeapons[WeaponToBeUnlocked] = true;
		TTGameInstance.Get()->GetSubsystem<UResourceManager>()->NotifyCanObtainFryResource();
		//Achievement of Totem
		TTGameInstance.Get()->TotemUnlocked(WeaponToBeUnlocked);
		return true;
	}
	return false;
}

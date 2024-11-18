// TinyTavern


#include "AmmoUpgrade.h"

#include "Components/Image.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UAmmoUpgrade::ShowUpgrades(TObjectPtr<UTT_GameInstance> TTGameInstance, bool UpgradeAvailable)
{
	Super::ShowUpgrades(TTGameInstance, UpgradeAvailable);

	int numberOfUpgrade = TTGameInstance.Get()->UpgradeTier[1];
	int numberOfRelics = TTGameInstance.Get()->RelicsInventory[1];
	CheckPlayerRelics(numberOfRelics, numberOfUpgrade);
	
	if(UpgradeAvailable)
	{
		UpgradeImage.Get()->SetBrush(UpgradeAvailableBrush);
	}
	else
	{
		UpgradeImage.Get()->SetBrush(UpgradeNormalBrush);
	}
	//If Last Level, show relic and upgrade symbol
	if(numberOfUpgrade == 4)
	{
		SetVisibility(ESlateVisibility::Hidden);
		CompletedAllTiers = true;
		//Eliminate all commands of the NPC that the player has in inventory
		for(int i = TTGameInstance.Get()->AcceptedOrders.Num()-1; i >= 0; i--)
		{
			if(TTGameInstance.Get()->AcceptedOrders[i].NPCTag == FGameplayTags::Get().NPC_Gailor)
			{
				TTGameInstance.Get()->AcceptedOrders.Remove(TTGameInstance.Get()->AcceptedOrders[i]);
			}
		}
		for(int i = TTGameInstance.Get()->FinishedOrders.Num()-1; i >= 0; i--)
		{
			if(TTGameInstance.Get()->FinishedOrders[i].NPCTag == FGameplayTags::Get().NPC_Gailor)
			{
				TTGameInstance.Get()->FinishedOrders.Remove(TTGameInstance.Get()->FinishedOrders[i]);
			}
		}
	}
}

bool UAmmoUpgrade::ExecuteUpgrade(TObjectPtr<UTT_GameInstance> TTGameInstance)
{
	Super::ExecuteUpgrade(TTGameInstance);
	int numberOfRelics = TTGameInstance.Get()->RelicsInventory[1];
	int numberOfUpgrade = TTGameInstance.Get()->UpgradeTier[1];

	switch (numberOfUpgrade)
	{
	case 0:
		if(numberOfRelics >= 1)
		{
			UpgradeAmmo(TTGameInstance, Upgrade1ValueToAdd);
			ShowUpgrades(TTGameInstance, false);
			ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Gailor, TTGameInstance);
			CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[1], TTGameInstance.Get()->UpgradeTier[1]);
			return true;
		}
		break;
	case 1:
		if(numberOfRelics >= 2)
		{
			UpgradeIngredient(TTGameInstance, Upgrade2ValueToAdd);
			ShowUpgrades(TTGameInstance, false);
			ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Gailor, TTGameInstance);
			CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[1], TTGameInstance.Get()->UpgradeTier[1]);
			return true;
		}
		break;
	case 2:
		if(numberOfRelics >= 3)
		{
			UpgradeAmmo(TTGameInstance, Upgrade3ValueToAdd);
			ShowUpgrades(TTGameInstance, false);
			ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Gailor, TTGameInstance);
			CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[1], TTGameInstance.Get()->UpgradeTier[1]);
			return true;
		}
		break;
	case 3:
		if(numberOfRelics == 4)
		{
			UpgradeIngredient(TTGameInstance, Upgrade4ValueToAdd);
			ShowUpgrades(TTGameInstance, false);
			ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Gailor, TTGameInstance);
			CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[1], TTGameInstance.Get()->UpgradeTier[1]);
			return true;
		}
		break;
	default: ;
	}
	return false;
}

void UAmmoUpgrade::UpgradeAmmo(TObjectPtr<UTT_GameInstance> TTGameInstance, float UpgradeNewValue)
{
	for(int i = 0; i < TTGameInstance.Get()->AttributesData.Num(); i++)
	{
		if(TTGameInstance.Get()->AttributesData[i].NameOfAttribute == MAXGRILLEDAMMO ||
			TTGameInstance.Get()->AttributesData[i].NameOfAttribute == MAXBOILAMMO ||
			TTGameInstance.Get()->AttributesData[i].NameOfAttribute == MAXFRYAMMO)
		{
			float currentMaxAmmo = 0;
			TTGameInstance.Get()->AttributesData[i].Attribute.ModifierMagnitude.GetStaticMagnitudeIfPossible(0,currentMaxAmmo);
			TTGameInstance.Get()->AttributesData[i].Attribute.ModifierMagnitude =
				FGameplayEffectModifierMagnitude(currentMaxAmmo + currentMaxAmmo * UpgradeNewValue);
		}
	}
	//Save Upgrade
	TTGameInstance.Get()->UpgradeTier[1] = TTGameInstance.Get()->UpgradeTier[1] + 1;
	UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel+1);
}

void UAmmoUpgrade::UpgradeIngredient(TObjectPtr<UTT_GameInstance> TTGameInstance, float UpgradeNewValue)
{
	for(int i = 0; i < TTGameInstance.Get()->AttributesData.Num(); i++)
	{
		if(TTGameInstance.Get()->AttributesData[i].NameOfAttribute == MAXCHEESEAMMO ||
			TTGameInstance.Get()->AttributesData[i].NameOfAttribute == MAXJAMAMMO ||
			TTGameInstance.Get()->AttributesData[i].NameOfAttribute == MAXWINEAMMO)
		{
			float currentMaxIngredient = 0;
			TTGameInstance.Get()->AttributesData[i].Attribute.ModifierMagnitude.GetStaticMagnitudeIfPossible(0,currentMaxIngredient);
			TTGameInstance.Get()->AttributesData[i].Attribute.ModifierMagnitude =
				FGameplayEffectModifierMagnitude(currentMaxIngredient + UpgradeNewValue);
		}
	}
	//Save Upgrade
	TTGameInstance.Get()->UpgradeTier[1] = TTGameInstance.Get()->UpgradeTier[1] + 1;
	UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].CurrentNPCSatisfactionLevel+1);
}

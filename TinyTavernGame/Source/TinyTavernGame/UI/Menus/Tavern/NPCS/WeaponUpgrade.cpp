// TinyTavern


#include "WeaponUpgrade.h"

#include "Components/Image.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UWeaponUpgrade::ShowUpgrades(TObjectPtr<UTT_GameInstance> TTGameInstance, bool UpgradeAvailable)
{
	Super::ShowUpgrades(TTGameInstance, UpgradeAvailable);

	int numberOfUpgrade = TTGameInstance.Get()->UpgradeTier[2];
	int numberOfRelics = TTGameInstance.Get()->RelicsInventory[2];
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
	if(numberOfUpgrade == 3)
	{
		SetVisibility(ESlateVisibility::Hidden);
		CompletedAllTiers = true;
		//Eliminate all commands of the NPC that the player has in inventory
		for(int i = TTGameInstance.Get()->AcceptedOrders.Num()-1; i >= 0; i--)
		{
			if(TTGameInstance.Get()->AcceptedOrders[i].NPCTag == FGameplayTags::Get().NPC_Gmith)
			{
				TTGameInstance.Get()->AcceptedOrders.Remove(TTGameInstance.Get()->AcceptedOrders[i]);
			}
		}
		for(int i = TTGameInstance.Get()->FinishedOrders.Num()-1; i >= 0; i--)
		{
			if(TTGameInstance.Get()->FinishedOrders[i].NPCTag == FGameplayTags::Get().NPC_Gmith)
			{
				TTGameInstance.Get()->FinishedOrders.Remove(TTGameInstance.Get()->FinishedOrders[i]);
			}
		}
	}
}

bool UWeaponUpgrade::ExecuteUpgrade(TObjectPtr<UTT_GameInstance> TTGameInstance)
{
	Super::ExecuteUpgrade(TTGameInstance);
	int numberOfRelics = TTGameInstance.Get()->RelicsInventory[2];
	int numberOfUpgrade = TTGameInstance.Get()->UpgradeTier[2];

	switch (numberOfUpgrade)
	{
	case 0:
		if(numberOfRelics >= 1)
		{
			UpgradeWeapons(TTGameInstance,1);
			ShowUpgrades(TTGameInstance, false);
			ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Gmith, TTGameInstance);
			CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[2], TTGameInstance.Get()->UpgradeTier[2]);
			return true;
		}
		break;
	case 1:
		if(numberOfRelics >= 2)
		{
			UpgradeWeapons(TTGameInstance,2);
			ShowUpgrades(TTGameInstance, false);
			ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Gmith, TTGameInstance);
			CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[2], TTGameInstance.Get()->UpgradeTier[2]);
			return true;
		}
		break;
	case 2:
		if(numberOfRelics >= 3)
		{
			UpgradeWeapons(TTGameInstance,3);
			ShowUpgrades(TTGameInstance, false);
			ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Gmith, TTGameInstance);
			CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[2], TTGameInstance.Get()->UpgradeTier[2]);
			return true;
		}
		break;
	default: ;
	}
	return false;
}

void UWeaponUpgrade::UpgradeWeapons(TObjectPtr<UTT_GameInstance> TTGameInstance, int Number)
{
	switch (Number)
	{
	case 1:
		TTGameInstance.Get()->WeaponsLevel[FGameplayTags::Get().Weapon_Range_Grill] = FWeaponsLevel::LVL2;
		break;
	case 2:
		TTGameInstance.Get()->WeaponsLevel[FGameplayTags::Get().Weapon_Range_Boil] = FWeaponsLevel::LVL2;
		break;
	case 3:
		TTGameInstance.Get()->WeaponsLevel[FGameplayTags::Get().Weapon_Range_Fry] = FWeaponsLevel::LVL2;
		break;
	}
	//Save Upgrade
	TTGameInstance.Get()->UpgradeTier[2] = TTGameInstance.Get()->UpgradeTier[2] + 1;
	UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].CurrentNPCSatisfactionLevel + 1);
}

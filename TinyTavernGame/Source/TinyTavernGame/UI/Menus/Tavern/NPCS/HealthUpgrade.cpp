// TinyTavern


#include "HealthUpgrade.h"

#include "Components/Image.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UHealthUpgrade::ShowUpgrades(TObjectPtr<UTT_GameInstance> TTGameInstance, bool UpgradeAvailable)
{
	Super::ShowUpgrades(TTGameInstance, UpgradeAvailable);
	
	int numberOfUpgrade = TTGameInstance.Get()->UpgradeTier[0];
	int numberOfRelics = TTGameInstance.Get()->RelicsInventory[0];
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
			if(TTGameInstance.Get()->AcceptedOrders[i].NPCTag == FGameplayTags::Get().NPC_Goctor)
			{
				TTGameInstance.Get()->AcceptedOrders.Remove(TTGameInstance.Get()->AcceptedOrders[i]);
			}
		}
		for(int i = TTGameInstance.Get()->FinishedOrders.Num()-1; i >= 0; i--)
		{
			if(TTGameInstance.Get()->FinishedOrders[i].NPCTag == FGameplayTags::Get().NPC_Goctor)
			{
				TTGameInstance.Get()->FinishedOrders.Remove(TTGameInstance.Get()->FinishedOrders[i]);
			}
		}
	}
}

bool UHealthUpgrade::ExecuteUpgrade(TObjectPtr<UTT_GameInstance> TTGameInstance)
{
	Super::ExecuteUpgrade(TTGameInstance);
	int numberOfRelics = TTGameInstance.Get()->RelicsInventory[0];
	int numberOfUpgrade = TTGameInstance.Get()->UpgradeTier[0];

	switch (numberOfUpgrade)
	{
		case 0:
				if(numberOfRelics >= 1)
				{
					UpgradeHealth(TTGameInstance, Upgrade1ValueToAdd);
					ShowUpgrades(TTGameInstance, false);
					ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Goctor, TTGameInstance);
					CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[0], TTGameInstance.Get()->UpgradeTier[0]);
					return true;
				}
			break;
		case 1:
				if(numberOfRelics >= 2)
				{
					UpgradeHealth(TTGameInstance, Upgrade2ValueToAdd);
					ShowUpgrades(TTGameInstance, false);
					ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Goctor, TTGameInstance);
					CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[0], TTGameInstance.Get()->UpgradeTier[0]);
					return true;
				}
			break;
		case 2:
				if(numberOfRelics == 3)
				{
					UpgradeHealth(TTGameInstance, Upgrade3ValueToAdd);
					ShowUpgrades(TTGameInstance, false);
					ResetNPCOrdersAndQuestBoardOrder(FGameplayTags::Get().NPC_Goctor, TTGameInstance);
					CheckPlayerRelics(TTGameInstance.Get()->RelicsInventory[0], TTGameInstance.Get()->UpgradeTier[0]);
					return true;
				}
			break;
		default: ;
	}
	return false;
}

void UHealthUpgrade::UpgradeHealth(TObjectPtr<UTT_GameInstance> TTGameInstance, float UpgradeNewValue)
{
	for(int i = 0; i < TTGameInstance.Get()->AttributesData.Num(); i++)
	{
		if(TTGameInstance.Get()->AttributesData[i].NameOfAttribute == MAXHEALTH)
		{
			float currentMaxHealth = 0;
			TTGameInstance.Get()->AttributesData[i].Attribute.ModifierMagnitude.GetStaticMagnitudeIfPossible(0,currentMaxHealth);
			TTGameInstance.Get()->AttributesData[i].Attribute.ModifierMagnitude =
				FGameplayEffectModifierMagnitude(currentMaxHealth + UpgradeNewValue);
			//HEALTH == MAXHEALTH
			TTGameInstance.Get()->AttributesData[i+1].Attribute.ModifierMagnitude = TTGameInstance.Get()->AttributesData[i].Attribute.ModifierMagnitude;
			//Save Upgrade
			TTGameInstance.Get()->UpgradeTier[0] = TTGameInstance.Get()->UpgradeTier[0] + 1;
			break;
		}
	}
	UpdateSatisfactionBar(TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].CurrentNPCSatisfactionLevel + 1);
}


// TinyTavern


#include "UpgradeUI.h"

#include "GameplayTagContainer.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/SpinningWidget.h"

void UUpgradeUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerHasRelic = false;
}

void UUpgradeUI::UpdateSatisfactionBar(int Level)
{
	if(!FoodUpgrade)
	{
		int CurrentLevel = ++Level;
		CurrentSatisfactionBarPercentages.Empty();
		switch (CurrentLevel)
		{
		case 1:
			CurrentSatisfactionBarPercentages = SatisfactionBarPercentagesLevel1;
			UpgradeBackground.Get()->SetBrush(SatisfactionBarImageLevel1);
			break;
		case 2:
			CurrentSatisfactionBarPercentages = SatisfactionBarPercentagesLevel2;
			UpgradeBackground.Get()->SetBrush(SatisfactionBarImageLevel2);
			break;
		case 3:
			CurrentSatisfactionBarPercentages = SatisfactionBarPercentagesLevel3;
			UpgradeBackground.Get()->SetBrush(SatisfactionBarImageLevel3);
			break;
		case 4:
			if(HasLevel4)
			{
				CurrentSatisfactionBarPercentages = SatisfactionBarPercentagesLevel4;
				UpgradeBackground.Get()->SetBrush(SatisfactionBarImageLevel4);
			}
			break;
		}
		//Achievement for obtaining all upgrades
		TObjectPtr<UTT_GameInstance> ttGameInstance = Cast<UTT_GameInstance>(GetWorld()->GetGameInstance());
		if (ttGameInstance)
		{
			ttGameInstance.Get()->UnlockedUpgrade();
		}
	}
	else
	{
		CurrentSatisfactionBarPercentages = SatisfactionBarPercentagesLevel1;
		UpgradeBackground.Get()->SetBrush(SatisfactionBarImageLevel1);
	}
}

float UUpgradeUI::GetPercentageToFill(int NumberOfStars)
{
	if(NumberOfStars != 0)
	{
		return CurrentSatisfactionBarPercentages[NumberOfStars-1];
	}
	return 0;
}

void UUpgradeUI::AddPercentNormalSatisfactionbar(float NumberOfStars)
{
	if(NumberOfStars != 0)
	{
		NPCSatisfactionBar.Get()->SetPercent(CurrentSatisfactionBarPercentages[NumberOfStars-1]);
	}
	else
	{
		NPCSatisfactionBar.Get()->SetPercent(0);
	}
}

void UUpgradeUI::AddPercentFutureSatisfactionbar(float NumberOfStars)
{
	if(NumberOfStars != 0)
	{
		NPCFutureSatisfactionBar.Get()->SetPercent(CurrentSatisfactionBarPercentages[NumberOfStars-1]);
	}
	else
	{
		NPCFutureSatisfactionBar.Get()->SetPercent(0);
	}
}

void UUpgradeUI::ShowLight()
{
	SpinningLight.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUpgradeUI::HideLight()
{
	SpinningLight.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UUpgradeUI::ResetNPCOrdersAndQuestBoardOrder(FGameplayTag NPCTag, TObjectPtr<UTT_GameInstance> TTGameInstance)
{
	if(NPCTag == FGameplayTags::Get().NPC_Goctor)
	{
		TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Goctor].NPCOrders.Empty();
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[0] = -1;
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[1] = -1;
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[2] = -1;
	}
	else if(NPCTag == FGameplayTags::Get().NPC_Gailor)
	{
		TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gailor].NPCOrders.Empty();
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[3] = -1;
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[4] = -1;
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[5] = -1;
	}
	else if(NPCTag == FGameplayTags::Get().NPC_Gmith)
	{
		TTGameInstance.Get()->NPCInformationMap[FGameplayTags::Get().NPC_Gmith].NPCOrders.Empty();
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[6] = -1;
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[7] = -1;
		TTGameInstance.Get()->PositionOfCommandsInQuestBoard[8] = -1;
	}
}

void UUpgradeUI::CheckPlayerRelics(int NumberOfRelics, int NumberOfUpgrade)
{
	if(NumberOfRelics > NumberOfUpgrade)
	{
		RelicImage.Get()->SetColorAndOpacity(FColor::White);
		ShowLight();
		PlayerHasRelic = true;
	}
	else
	{
		RelicImage.Get()->SetColorAndOpacity(FColor::Black);
		HideLight();
		PlayerHasRelic = false;;
	}
}

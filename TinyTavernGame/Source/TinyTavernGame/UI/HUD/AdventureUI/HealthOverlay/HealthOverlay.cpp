// TinyTavern


#include "HealthOverlay.h"
#include "../ProgressBarPoint.h"
#include "Components/HorizontalBox.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/OverlayWidgetController.h"

void UHealthOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	if(GameInstance.Get()->IsGavernDay && GameInstance.Get()->GavernUpgrade == EGavernUpgrades::SHIELD)
	{
		CurrentArmor = GameInstance.Get()->NumberOfHeartShield;
		for(int i=0; i < CurrentArmor/2; i++)
		{
			UProgressBarPoint* Widget = CreateWidget<UProgressBarPoint>(GetWorld(), ArmorWidgetClass);
			ArmorHorizontalBox.Get()->AddChildToHorizontalBox(Widget);
			ArmorPoints.Add(Widget);
		}
	}
}

void UHealthOverlay::WidgetControllerSet_Implementation()
{
	Super::WidgetControllerSet_Implementation();

	Cast<UOverlayWidgetController>(WidgetController)->OnHealthChange.AddDynamic(this,&UHealthOverlay::UpdateHealth);
	Cast<UOverlayWidgetController>(WidgetController)->OnMaxHealthChange.AddDynamic(this, &UHealthOverlay::UpdateMaxHealth);
}

void UHealthOverlay::UpdateHealth(float newAttributeValue)
{
	int Damage = CurrentHealth - newAttributeValue; //Gets the damage received by player (if less than 0 player has healed)
	
	if(ShouldUpdateOverlay)//Only to avoid the double call of Update Health meant fot other purposes. Should enter in first call
	{
		//If player has Armor
		if(CurrentArmor > 0)
		{
			int NewArmor = CurrentArmor - Damage;
			if(NewArmor >= 0)
			{
				bool Even = (NewArmor % 2 == 0);
				int ArmorHearthsToDraw = NewArmor/2;
				for(int i=0; i < ArmorPoints.Num(); i++)
				{
					if(i < ArmorHearthsToDraw) 
					{
						ArmorPoints[i]->UpdateImage(Full);
					}
					else if(!Even)
					{
						ArmorPoints[i]->UpdateImage(Half);
					}
					else
					{
						ArmorPoints[i]->SetVisibility(ESlateVisibility::Hidden);
					}
				}
				CurrentArmor = NewArmor;
			}
			else
			{
				CurrentArmor = 0;
			}
			ShouldUpdateOverlay = false;
		}
		//If player doesn't have armor
		else if(!HealthPoints.IsEmpty())
		{
			bool Even = (static_cast<int>(newAttributeValue) % 2 == 0);
			
			int NewHealth = CurrentHealth - Damage;
			if(NewHealth >= 0)
			{
				int HearthsToDraw = NewHealth/2;
				for(int i=0; i < HealthPoints.Num(); i++)
				{
					if(i < HearthsToDraw) 
					{
						HealthPoints[i]->UpdateImage(Full);
					}
					else if(!Even && i == HearthsToDraw)
					{
						HealthPoints[i]->UpdateImage(Half);
					}
					else
					{
						HealthPoints[i]->UpdateImage(Empty);
					}
				}
				CurrentHealth = NewHealth;
				ShouldUpdateOverlay = false;
			}
		}
	}
	else
	{
		ShouldUpdateOverlay = true;
	}
	
	CurrentHealth = newAttributeValue;
}
	

void UHealthOverlay::UpdateMaxHealth(float newAttributeValue)
{
	if(CurrentMaxHealth < newAttributeValue)
	{
		int widgetsToCreate = newAttributeValue/2 - CurrentMaxHealth;
		for (int i = 0; i < widgetsToCreate; i++)
		{
			UProgressBarPoint* Widget = CreateWidget<UProgressBarPoint>(GetWorld(),HealthPointWidgetClass.Get());
			HearthsHorizontalBox.Get()->AddChildToHorizontalBox(Widget);
			HealthPoints.Add(Widget);
		}
		CurrentMaxHealth = newAttributeValue;
	}
}


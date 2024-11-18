// TinyTavern


#include "ArmoryOverlay.h"

#include "AmmoBar.h"
#include "AmmoCounter.h"
#include "IngredientType.h"
#include "WeaponType.h"
#include "Components/ScaleBox.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"
#include "TinyTavernGame/UI/HUD/OverlayWidgetController.h"


void UArmoryOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	CheckUnlockedWeapons();
}

void UArmoryOverlay::WidgetControllerSet_Implementation()
{	
	UOverlayWidgetController* OverlayWidget = Cast<UOverlayWidgetController>(WidgetController);
	WeaponAmmoBar->SetWidgetController(OverlayWidget);
	IngredientWidget->SetWidgetController(OverlayWidget);
	OverlayWidget->OnWeaponChange.AddDynamic(this,&UArmoryOverlay::UpdateWeapon);
	OverlayWidget->OnWeaponUnlocked.AddDynamic(this, &UArmoryOverlay::CheckUnlockedWeapons);
	WeaponAmmoBar->OnAmmoUpdate.AddDynamic(this, &UArmoryOverlay::UpdateAmmoCounter);
}

void UArmoryOverlay::ShowRangeWeaponButton()
{
	RangeButtonSizeBox->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UArmoryOverlay::UpdateWeapon(FGameplayTag newWeapon)
{
	if(newWeapon.MatchesTag(WeaponTag))
	{
		WeaponWidget->UpdateImage(newWeapon);
		WeaponAmmoBar->UpdateAmmoSource(newWeapon);
	}
	else if(newWeapon.MatchesTag(IngredientTag))
	{
		IngredientWidget->UpdateImage(newWeapon);
		IngredientWidget->UpdateIngredientAmmoSource(newWeapon);
	}
}

void UArmoryOverlay::UpdateAmmoCounter(float CurrentAmmo, float MaxCurrentAmmo)
{
	WeaponAmmoCounter->UpdateAmmo(CurrentAmmo,MaxCurrentAmmo);
}

void UArmoryOverlay::CheckUnlockedWeapons()
{
	UTT_GameInstance* GameInstance = Cast<UTT_GameInstance>(GetGameInstance());
	if(!GameInstance->UnlockedWeapons[FGameplayTags::Get().Weapon_Range_Boil])
	{
		RangeButtonSizeBox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		RangeButtonSizeBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if(!GameInstance->UnlockedWeapons[FGameplayTags::Get().Weapon_Ingredient_Jam])
	{
		IngredientWidget.Get()->SetCanShowButton(false);
	}
	else
	{
		IngredientWidget.Get()->SetCanShowButton(true);
		IngredientWidget.Get()->ButtonSizeBox.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if(GameInstance->CurrentLevel >= 0)
	{
		if(GameInstance->CurrentLevel == 0)
		{
			IngredientWidget.Get()->SetCanShowButton(true);
		}
		else
		{
			IngredientWidget.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}






// TinyTavern


#include "OverlayWidgetController.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Player/TTPlayerController.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

void UOverlayWidgetController::BeginDestroy()
{
	Super::BeginDestroy();
	/*
	//Subscribe to WeaponComponent OnWeaponChange Delegate
	WeaponComponent->OnWeaponChange.RemoveDynamic(this, &UOverlayWidgetController::WeaponChange);
	//Subscribe to PlayerController, Settings Button
	ATTPlayerController* PC = Cast<ATTPlayerController>(PlayerController);
	PC->OnSettingsMenu.RemoveDynamic(this, &UOverlayWidgetController::ManageSettingsMenu);
	PC->OnQuestMenuOpenOrClose.RemoveDynamic(this, &UOverlayWidgetController::OpenOrCloseQuestMenu);
	PC->OnQuestNextOrLast.RemoveDynamic(this, &UOverlayWidgetController::NextOrLastQuest);
	PC->OnInteract.RemoveDynamic(this, &UOverlayWidgetController::SelectRecipe);
	*/
}

void UOverlayWidgetController::BroadCastInitialValues()
{
	const UTT_AttributeSet* TT_AttributeSet = CastChecked<UTT_AttributeSet>(AttributeSet);
	FGameplayTagContainer TagContainer;
	AbilitySystemComponent.Get()->GetOwnedGameplayTags(TagContainer);
	OnHealthChange.Broadcast(TT_AttributeSet->GetHealth());
	OnMaxHealthChange.Broadcast(TT_AttributeSet->GetMaxHealth());
	OnGrilledAmmoChange.Broadcast(TT_AttributeSet->GetGrilledAmmo());
	OnMaxGrilledAmmoChange.Broadcast(TT_AttributeSet->GetMaxGrilledAmmo());
	OnBoiledAmmoChange.Broadcast(TT_AttributeSet->GetBoiledAmmo());
	OnMaxBoiledAmmoChange.Broadcast(TT_AttributeSet->GetMaxBoiledAmmo());
	OnFriedAmmoChange.Broadcast(TT_AttributeSet->GetFriedAmmo());
	OnMaxFriedAmmoChange.Broadcast(TT_AttributeSet->GetMaxFriedAmmo());
	OnWineAmmoChange.Broadcast(TT_AttributeSet->GetWineAmmo());
	OnMaxWineAmmoChange.Broadcast(TT_AttributeSet->GetMaxWineAmmo());
	OnCheeseAmmoChange.Broadcast(TT_AttributeSet->GetCheeseAmmo());
	OnMaxCheeseAmmoChange.Broadcast(TT_AttributeSet->GetMaxCheeseAmmo());
	OnJamAmmoChange.Broadcast(TT_AttributeSet->GetJamAmmo());
	OnMaxJamAmmoChange.Broadcast(TT_AttributeSet->GetMaxJamAmmo());
	OnPoisonResistanceChange.Broadcast(TT_AttributeSet->GetPoisonResistance());
	OnMaxPoisonResistanceChange.Broadcast(TT_AttributeSet->GetMaxPoisonResistance());
	OnWeaponChange.Broadcast(WeaponComponent->GetCurrentRangeWeaponTag());
	OnWeaponChange.Broadcast(WeaponComponent->GetCurrentIngredientWeaponTag());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UTT_AttributeSet* TT_AttributeSet = CastChecked<UTT_AttributeSet>(AttributeSet);
	//Subscribe to Attribute's Value Change Delegate
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetGrilledAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::GrilledAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetMaxGrilledAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::MaxGrilledAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetBoiledAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::BoiledAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetMaxBoiledAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::MaxBoiledAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetFriedAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::FriedAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetMaxFriedAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::MaxFriedAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			TT_AttributeSet->GetWineAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::WineAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetMaxWineAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::MaxWineAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			TT_AttributeSet->GetCheeseAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::CheeseAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetMaxCheeseAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::MaxCheeseAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			TT_AttributeSet->GetJamAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::JamAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetMaxJamAmmoAttribute()).AddUObject(this, &UOverlayWidgetController::MaxJamAmmoChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			TT_AttributeSet->GetPoisonResistanceAttribute()).AddUObject(this, &UOverlayWidgetController::PoisonResistanceChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TT_AttributeSet->GetMaxPoisonResistanceAttribute()).AddUObject(this, &UOverlayWidgetController::MaxPoisonResistanceChange);
	//Subscribe to WeaponComponent OnWeaponChange Delegate
	WeaponComponent->OnWeaponChange.AddDynamic(this, &UOverlayWidgetController::WeaponChange);
	WeaponComponent->OnWeaponUnlocked.AddDynamic(this, &UOverlayWidgetController::WeaponUnlocked);
	//Subscribe to PlayerController, Settings Button
	ATTPlayerController* PC = Cast<ATTPlayerController>(PlayerController);
	PC->OnSettingsMenu.AddDynamic(this, &UOverlayWidgetController::ManageSettingsMenu);
	PC->OnQuestMenuOpenOrClose.AddDynamic(this, &UOverlayWidgetController::OpenOrCloseQuestMenu);
	PC->OnQuestNextOrLast.AddDynamic(this, &UOverlayWidgetController::NextOrLastQuest);
	Player = Cast<APlayerCharacter>(PC->GetCharacter());
	Player.Get()->OnHudVisibilityChange.AddDynamic(this, &UOverlayWidgetController::HudVisibilityChange);
	Player.Get()->OnInventoryFullChange.AddDynamic(this, &UOverlayWidgetController::InventoryFullChange);
}

void UOverlayWidgetController::HealthChange(const FOnAttributeChangeData& Data) const
{
	OnHealthChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChange(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::GrilledAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnGrilledAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxGrilledAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnMaxGrilledAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::BoiledAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnBoiledAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxBoiledAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnMaxBoiledAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::FriedAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnFriedAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxFriedAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnMaxFriedAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::WineAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnWineAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxWineAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnMaxWineAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::CheeseAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnCheeseAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxCheeseAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnMaxCheeseAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::JamAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnJamAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxJamAmmoChange(const FOnAttributeChangeData& Data) const
{
	OnMaxJamAmmoChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::PoisonResistanceChange(const FOnAttributeChangeData& Data) const
{
	OnPoisonResistanceChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxPoisonResistanceChange(const FOnAttributeChangeData& Data) const
{
	OnMaxPoisonResistanceChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::WeaponChange(FGameplayTag newWeapon)
{
	OnWeaponChange.Broadcast(newWeapon);
}

void UOverlayWidgetController::WeaponUnlocked()
{
	OnWeaponUnlocked.Broadcast();
}

void UOverlayWidgetController::ManageSettingsMenu(bool InPauseMenu)
{
	OnSettingsMenu.Broadcast(InPauseMenu);
}

void UOverlayWidgetController::OpenOrCloseQuestMenu(bool Open)
{
	OnQuestMenu.Broadcast(Open);
}

void UOverlayWidgetController::NextOrLastQuest(bool Next)
{
	OnNextOrLastQuestSelection.Broadcast(Next);
}

void UOverlayWidgetController::SetHUDIsHidden(bool _HUDIsHidden)
{
	Player.Get()->SetHUDIsHidden(_HUDIsHidden);
}

void UOverlayWidgetController::HudVisibilityChange(bool ShouldHide)
{
	OnHudVisibilityChange.Broadcast(ShouldHide);
}

void UOverlayWidgetController::InventoryFullChange(bool InventoryFull)
{
	OnInventoryFullChange.Broadcast(InventoryFull);
}

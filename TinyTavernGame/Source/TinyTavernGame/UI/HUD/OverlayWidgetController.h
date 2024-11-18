// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTWidgetController.h"
#include "OverlayWidgetController.generated.h"

class APlayerCharacter;
//Delegates to broadcast the attributes changes to the widget.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCookbarChangeSignature, float, NewCookbar);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxCookbarChangeSignature, float, NewMaxCookbar);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrilledAmmoChangeSignature, float, newGrilledAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxGrilledAmmoChangeSignature, float, newMaxGrilledAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoiledAmmoChangeSignature, float, newBoiledAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxBoiledAmmoChangeSignature, float, newMaxBoiledAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriedAmmoChangeSignature, float, newFriedAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxFriedAmmoChangeSignature, float, newMaxFriedAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWineAmmoChangeSignature, float, newWineAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxWineAmmoChangeSignature, float, newMaxWineAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheeseAmmoChangeSignature, float, newCheeseAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxCheeseAmmoChangeSignature, float, newMaxCheeseAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJamAmmoChangeSignature, float, newJamAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxJamAmmoChangeSignature, float, newMaxJamAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoisonResistanceChangeSignature, float, newPoisonResistance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxPoisonResistanceChangeSignature, float, newMaxPoisonResistance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangeToWidgetSignature, FGameplayTag, equippedWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUnlockedToWidgetSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsMenuWidgetSignature, bool, InPauseMenu);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestsMenuOpenOrCloseWidgetSignature, bool, Open);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestsMenuNextOrLastWidgetSignature, bool, Next);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHudVisibilityChangeWidgetSignature, bool, shouldHide);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryFullChangeSignature, bool, InventoryFull = false);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class TINYTAVERNGAME_API UOverlayWidgetController : public UTTWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BeginDestroy() override;
	void SetHUDIsHidden(bool _HUDIsHidden);
	//Broadcast the Initial Values to the widgets that use this widget controller
	virtual void BroadCastInitialValues() override;
	//Bind callbacks to the functions that broadcast the attributes values changes to the widgets that use this controller
	virtual void BindCallbacksToDependencies() override;

	//Delegates to broadcast the new values of the attributes (Both to initialize them and later to update them)
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnCookbarChangeSignature OnHealthChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxCookbarChangeSignature OnMaxHealthChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnGrilledAmmoChangeSignature OnGrilledAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxGrilledAmmoChangeSignature OnMaxGrilledAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnBoiledAmmoChangeSignature OnBoiledAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxBoiledAmmoChangeSignature OnMaxBoiledAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnFriedAmmoChangeSignature OnFriedAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxFriedAmmoChangeSignature OnMaxFriedAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnWineAmmoChangeSignature OnWineAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxWineAmmoChangeSignature OnMaxWineAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnCheeseAmmoChangeSignature OnCheeseAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxCheeseAmmoChangeSignature OnMaxCheeseAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnJamAmmoChangeSignature OnJamAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxJamAmmoChangeSignature OnMaxJamAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPoisonResistanceChangeSignature OnPoisonResistanceChange;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxPoisonResistanceChangeSignature OnMaxPoisonResistanceChange;
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponChangeToWidgetSignature OnWeaponChange;
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnWeaponUnlockedToWidgetSignature OnWeaponUnlocked;
	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnSettingsMenuWidgetSignature OnSettingsMenu;
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnQuestsMenuOpenOrCloseWidgetSignature OnQuestMenu;
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnQuestsMenuNextOrLastWidgetSignature OnNextOrLastQuestSelection;
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnHudVisibilityChangeWidgetSignature OnHudVisibilityChange;
	UPROPERTY(BlueprintAssignable, Category = "HUD")
	FOnInventoryFullChangeSignature OnInventoryFullChange;
	
protected:
	//Functions to Broadcast the attribute's values changes to the widgets
	void HealthChange(const FOnAttributeChangeData& Data) const;
	void MaxHealthChange(const FOnAttributeChangeData& Data) const;
	void GrilledAmmoChange(const FOnAttributeChangeData& Data) const;
	void MaxGrilledAmmoChange(const FOnAttributeChangeData& Data) const;
	void BoiledAmmoChange(const FOnAttributeChangeData& Data) const;
	void MaxBoiledAmmoChange(const FOnAttributeChangeData& Data) const;
	void FriedAmmoChange(const FOnAttributeChangeData& Data) const;
	void MaxFriedAmmoChange(const FOnAttributeChangeData& Data) const;
	void WineAmmoChange(const FOnAttributeChangeData& Data) const;
	void MaxWineAmmoChange(const FOnAttributeChangeData& Data) const;
	void CheeseAmmoChange(const FOnAttributeChangeData& Data) const;
	void MaxCheeseAmmoChange(const FOnAttributeChangeData& Data) const;
	void JamAmmoChange(const FOnAttributeChangeData& Data) const;
	void MaxJamAmmoChange(const FOnAttributeChangeData& Data) const;
	void PoisonResistanceChange(const FOnAttributeChangeData& Data) const;
	void MaxPoisonResistanceChange(const FOnAttributeChangeData& Data) const;

	//Function to broadcast the weapon change to the widgets;
	UFUNCTION()
	void WeaponChange(FGameplayTag newWeapon);

	UFUNCTION()
	void WeaponUnlocked();
	
	//Function to Open/Close Settings Menu
	UFUNCTION()
	void ManageSettingsMenu(bool InPauseMenu);

	//Open or Close Quests Menu
	UFUNCTION()
	void OpenOrCloseQuestMenu(bool Open);

	//Navigate trough the Quest Menu
	UFUNCTION()
	void NextOrLastQuest(bool Next);

	//Function to broadcast when the HUD shoul Hide or be Visible;
	UFUNCTION()
	void HudVisibilityChange(bool ShouldHide);
	//Function to broadcast when Inventory is full
	UFUNCTION()
	void InventoryFullChange(bool InventoryFull);

private:
	TObjectPtr<APlayerCharacter> Player;
};

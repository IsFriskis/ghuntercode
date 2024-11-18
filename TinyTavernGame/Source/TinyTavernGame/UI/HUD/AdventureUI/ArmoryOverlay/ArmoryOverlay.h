// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "ArmoryOverlay.generated.h"

class UScaleBox;
class UIngredientType;
class UAmmoCounter;
class UHorizontalBox;
class UAmmoBar;
class UWeaponType;
class UProgressBarPoint;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UArmoryOverlay : public UTTUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void WidgetControllerSet_Implementation() override;
	void ShowRangeWeaponButton();
protected:
	UFUNCTION()
	void UpdateWeapon(FGameplayTag newWeapon);
	UFUNCTION()
	void UpdateAmmoCounter(float CurrentAmmo, float MaxCurrentAmmo);
	UFUNCTION()
	void CheckUnlockedWeapons();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWeaponType> WeaponWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UIngredientType> IngredientWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UAmmoBar> WeaponAmmoBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UAmmoCounter> WeaponAmmoCounter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UScaleBox> RangeButtonSizeBox;
	
	UPROPERTY(EditAnywhere, Category = "TT| Weapons")
	FGameplayTag WeaponTag;
	UPROPERTY(EditAnywhere, Category = "TT| Ingredients")
	FGameplayTag IngredientTag;
	
	
	
	
};

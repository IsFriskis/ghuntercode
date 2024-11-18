// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/UI/HUD/AdventureUI/BaseBar.h"
#include "AmmoBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoUpdate,float,AmmoCounter,float,MaxAmmoCounter);

class UTexture2D;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UAmmoBar : public UTTUserWidget
{
	GENERATED_BODY()
public:
	virtual void WidgetControllerSet_Implementation() override;
	void UpdateAmmoSource(FGameplayTag EquippedWeapon);
	void UpdateAmmo();
	float GetCurrentAmmo();
	float GetCurrentMaxAmmo();
	
	UFUNCTION()
	void SetBoilAmmo(float newAmmo);
	UFUNCTION()
	void SetMaxBoilAmmo(float newAmmo);
	UFUNCTION()
	void SetFryAmmo(float newAmmo);
	UFUNCTION()
	void SetMaxFryAmmo(float newAmmo);
	UFUNCTION()
	void SetGrillAmmo(float newAmmo);
	UFUNCTION()
	void SetMaxGrillAmmo(float newAmmo);
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAmmoUpdate OnAmmoUpdate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
	
	UPROPERTY(EditAnywhere, Category = "TT")
	FSlateBrush BoilImage;
	UPROPERTY(EditAnywhere, Category = "TT")
	FSlateBrush BoilFillImage;
	UPROPERTY(EditAnywhere, Category = "TT")
	FSlateBrush FryImage;
	UPROPERTY(EditAnywhere, Category = "TT")
	FSlateBrush FryFillImage;
	UPROPERTY(EditAnywhere, Category = "TT")
	FSlateBrush GrillImage;
	UPROPERTY(EditAnywhere, Category = "TT")
	FSlateBrush GrillFillImage;
	
private:
	FGameplayTag SelectedWeapon;
	float CurrentAmmo;
	float CurrentMaxAmmo;
	float BoilAmmo;
	float MaxBoilAmmo;
	float FryAmmo;
	float MaxFryAmmo;
	float GrillAmmo;
	float MaxGrillAmmo;
};

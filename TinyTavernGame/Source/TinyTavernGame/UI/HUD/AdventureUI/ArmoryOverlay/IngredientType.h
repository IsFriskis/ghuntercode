// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "WeaponType.h"
#include "IngredientType.generated.h"

/**
 * 
 */

class UImage;
class UScaleBox;
class UHorizontalBox;
class UTexture2D;
class UProgressBarPoint;

UCLASS()
class TINYTAVERNGAME_API UIngredientType : public UWeaponType
{
	GENERATED_BODY()
	
public:
	virtual void WidgetControllerSet_Implementation() override;
	virtual void NativeConstruct() override;
	
	void UpdateIngredientAmmoSource(FGameplayTag EquippedIngredient);
	void SetCanShowButton(bool CanShowButton);
	
	UFUNCTION()
	void UpdateCheeseAmmo(float newAttributeValue);
	UFUNCTION()
	void UpdateMaxCheeseAmmo(float newAttributeValue);
	UFUNCTION()
	void UpdateJamAmmo(float newAttributeValue);
	UFUNCTION()
	void UpdateMaxJamAmmo(float newAttributeValue);
	UFUNCTION()
	void UpdateWineAmmo(float newAttributeValue);
	UFUNCTION()
	void UpdateMaxWineAmmo(float newAttributeValue);
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BeltImage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBarPoint> AmmoSlot1;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBarPoint> AmmoSlot2;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBarPoint> AmmoSlot3;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBarPoint> AmmoSlot4;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBarPoint> AmmoSlot5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UScaleBox> ButtonSizeBox;

protected:

	void UpdateIngredientAmmo(float newAttributeValue);
	void UpdateMaxIngredientAmmo(float newAttributeValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredients")
	TObjectPtr<UTexture2D> FullCheeseImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredients")
	TObjectPtr<UTexture2D> FullWineImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredients")
	TObjectPtr<UTexture2D> FullJamImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredients")
	TObjectPtr<UTexture2D> Belt3Ingredients;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredients")
	TObjectPtr<UTexture2D> Belt4Ingredients;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredients")
	TObjectPtr<UTexture2D> Belt5Ingredients;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Ingredients")
	TSubclassOf<UProgressBarPoint> AmmoPointWidgetClass;
	
private:
	void UpdateAmmoImages(UTexture2D* Texture);
	TArray<TObjectPtr<UProgressBarPoint>> IngredientAmmoPoints;
	int CurrentIngredientAmmo = 0;
	int CurrentIngredientMaxAmmo = 3;
	FGameplayTag SelectedIngredient;
	float CheeseAmmo;
	float CheeseMaxAmmo;
	float JamAmmo;
	float JamMaxAmmo;
	float WineAmmo;
	float WineMaxAmmo;
	bool ShowButton = false;
};

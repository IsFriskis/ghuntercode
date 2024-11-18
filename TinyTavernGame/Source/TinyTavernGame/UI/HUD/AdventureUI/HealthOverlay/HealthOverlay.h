// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "HealthOverlay.generated.h"

class UTT_GameInstance;
struct FGameplayTag;
class UProgressBarPoint;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UHealthOverlay : public UTTUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;
	virtual void WidgetControllerSet_Implementation() override;
	
	UFUNCTION()
	void UpdateHealth(float newAttributeValue);
	UFUNCTION()
	void UpdateMaxHealth(float newAttributeValue);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HearthsHorizontalBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> PoisonHorizontalBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> ArmorHorizontalBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|WidgetSettings")
	TSubclassOf<UProgressBarPoint> HealthPointWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|WidgetSettings")
	TSubclassOf<UProgressBarPoint> ArmorWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|WidgetSettings")
	TSubclassOf<UProgressBarPoint> PoisonWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|WidgetSettings")
	FGameplayTag ShieldTag;
	
private:
	bool ShouldUpdateOverlay = true;
	TArray<TObjectPtr<UProgressBarPoint>> HealthPoints;
	TArray<TObjectPtr<UProgressBarPoint>> ArmorPoints;
	UPROPERTY()
	TObjectPtr<UTT_GameInstance> GameInstance;
	int CurrentMaxHealth = 0;
	int CurrentHealth = 0;
	int CurrentArmor = 0;
};

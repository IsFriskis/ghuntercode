// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "..\AdventureUI\BaseBar.h"
#include "EnemyCookBar.generated.h"

class UImage;
class UTexture2D;
class AEnemyBase;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UEnemyCookBar : public UTTUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnCookbarChanged(float Cookbar);

	UFUNCTION(BlueprintCallable)
	void SetProgressBarImages(FGameplayTag Tag);

	virtual void NativeConstruct() override;

	void InitializeUI(AEnemyBase* Enemy);

	void OnMaxBurnBarReached();

	void RunCookedAnimation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CookbarImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BrokenCookbarImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> CookbarProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> BurnbarProgressBar;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> BurningAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CookedGrillAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CookedBoilAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CookedFryAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TMap<FGameplayTag, UTexture2D*> CookingIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TMap<FGameplayTag, FLinearColor> FillColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| WidgetSettings")
	TMap<FGameplayTag, FLinearColor> TintColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Sounds")
	TObjectPtr<USoundBase> BrokenGlassSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT| Sounds")
	TObjectPtr<USoundBase> CookedSound;

private:
	AEnemyBase* EnemyActor;
	FGameplayTag CookMode;
	bool IsBurning;
};

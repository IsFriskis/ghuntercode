// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "TinyTavernGame/UI/TTUserWidget.h"
#include "UpgradeAnimationWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpgradeAnimationFinished);

UCLASS()
class TINYTAVERNGAME_API UUpgradeAnimationWidget : public UTTUserWidget
{
	GENERATED_BODY()

public:
	FOnUpgradeAnimationFinished OnUpgradeAnimationFinished;
	
	void ExecuteAnimation(){PlayAnimation(UpgradeAnimation);}
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteGrillCleaverAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteSmashAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteBoilAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteSkewerAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteFryAnimation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteShieldAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecuteExtraReputationAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ExecutePuffBalResistanceAnimation();
	
protected:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> UpgradeAnimation;

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};

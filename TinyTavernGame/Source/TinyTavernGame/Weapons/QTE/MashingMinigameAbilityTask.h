// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "MinigameAbilityTask.h"
#include "MashingMinigameAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMashingTimerFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMashingProgressComplete);

USTRUCT()
struct FMashingMinigameParams
{
	GENERATED_BODY()

	UPROPERTY()
	UGameplayAbility* OwningAbility;

	UPROPERTY()
	UQTEOverlay* WidgetInstance;

	float TimeWindow;
	float CorrectInputProgression;
	float ProgressionLostPerSecond;
	EButtonToPress ButtonToMash;
};

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMashingMinigameAbilityTask : public UMinigameAbilityTask
{
	GENERATED_BODY()
	
public:
	UMashingMinigameAbilityTask(const FObjectInitializer& ObjectInitializer);
	static TObjectPtr<UMashingMinigameAbilityTask> CreateMinigameTask(FMashingMinigameParams Params);

	UPROPERTY(BlueprintAssignable)
	FOnMashingTimerFinished OnTimerFinished;

	UPROPERTY(BlueprintAssignable)
	FOnMashingProgressComplete OnMinigameComplete;

protected:
	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	void OnInputPressed(EButtonToPress Button = EButtonToPress::INVALID);

private:
	float MashingProgress = 0;
	float ProgressionLostPerSecond = 0;
	float CorrectInputProgression = 0;
	float CurrentTimeLeft = 0;
	float TotalTimeMinigame = 0;
	float WorldTimeDilation = 0;
	float PlayerTimeDilation = 0;
};

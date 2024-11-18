// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "MinigameAbilityTask.h"
#include "SequenceMinigameAbilityTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputTimerFinished);

USTRUCT()
struct FSequenceMinigameParams
{
	GENERATED_BODY()

	UPROPERTY()
	UGameplayAbility* OwningAbility;

	UPROPERTY()
	UQTEOverlay* WidgetInstance;

	float TimeWindowPerInput;
	float DelayTimeBetweenInputs;
};

class UTTAnimNotify;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API USequenceMinigameAbilityTask : public UMinigameAbilityTask
{
	GENERATED_BODY()
	
public:
	USequenceMinigameAbilityTask(const FObjectInitializer& ObjectInitializer);
	static TObjectPtr<USequenceMinigameAbilityTask> CreateMinigameTask(FSequenceMinigameParams _Params);

	UPROPERTY(BlueprintAssignable)
	FOnInputTimerFinished OnTimerFinished;

protected:
	virtual void TickTask(float DeltaTime) override;
	
private:
	void ResetInputTimer();

	UFUNCTION()
	void SetDelayForNextInput();

	UFUNCTION()
	void OnInputPressed(EButtonToPress Button = EButtonToPress::INVALID);

	bool ButtonPressed = false;
	float RemainingTimeForInput = 0;
	float TimeWindowPerInput = 0;
	float DelayTimeBetweenInputs = 0;
	FTimerHandle TimerHandle;

	float WorldTimeDilation;
	float PlayerTimeDilation;

	TArray<TObjectPtr<UAnimNotify>> AnimNotifies;
};

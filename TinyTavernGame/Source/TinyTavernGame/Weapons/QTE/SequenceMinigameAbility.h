// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "MinigameAbility.h"
#include "SequenceMinigameAbility.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API USequenceMinigameAbility : public UMinigameAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (ToolTip = "Amount of correct inputs needed to win"), meta = (Categories = "Enemy"))
	TMap<FGameplayTag, float> InputsRequiredToWin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (ToolTip = "Amount of failed inputs needed to lose"), meta = (Categories = "Enemy"))
	TMap<FGameplayTag, float> InputsFailedToLose;
;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (ToolTip = "Time window for each button press"), meta = (Categories = "Enemy"))
	TMap<FGameplayTag, float> TimeWindowPerButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (ToolTip = "The duration of the pause between input presses, in seconds"), meta = (Categories = "Enemy"))
	float DelayBetweenGeneratedInputs;


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	UFUNCTION()
	virtual void OnButtonPressed(EButtonToPress Button) override;

	UFUNCTION()
	void OnTimerRanOut();

	UFUNCTION()
	void GenerateNextInput();

	UFUNCTION()
	void SetDelayForNextInput();

	void ActivateMinigame();

private: 
	int GetAnimationIndex(EButtonToPress Button);

	int CurrentSuccessfulInputs = 0;
	int CurrentFailedInputs = 0;
	FTimerHandle TimerHandle;
};

// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "MinigameAbility.h"
#include "MashingMinigameAbility.generated.h"


/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UMashingMinigameAbility : public UMinigameAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
	TMap<FGameplayTag, float> MinigameTimeWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
	TMap<FGameplayTag, float> CorrectInputGain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
	TMap<FGameplayTag, float> ProgressionLostPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Gameplay", meta = (Categories = "Enemy"))
	EButtonToPress ButtonToMash;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	UFUNCTION()
	virtual void OnButtonPressed(EButtonToPress Button) override;
	
	UFUNCTION()
	void OnTimeRanOut();
	
	UFUNCTION()
	void OnMinigameSuccess();

	void ActivateMinigame();

};

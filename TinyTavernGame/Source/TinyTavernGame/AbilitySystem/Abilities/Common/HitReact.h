// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "HitReact.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UHitReact : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TT|Effects")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UFUNCTION()
	void OnAnimationEnd();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> HitReactAnim;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;
};

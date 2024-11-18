// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CheckDie.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UCheckDie : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Weapon Effect")
	TSubclassOf<UGameplayEffect> StunEffect;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
};

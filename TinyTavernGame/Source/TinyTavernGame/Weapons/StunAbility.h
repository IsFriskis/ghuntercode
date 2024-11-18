// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "StunAbility.generated.h"

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UStunAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};

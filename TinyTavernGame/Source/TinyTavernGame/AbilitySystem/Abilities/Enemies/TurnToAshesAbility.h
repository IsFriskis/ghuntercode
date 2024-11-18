// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "PlayAnimationAbility.h"
#include "TurnToAshesAbility.generated.h"

class AEnemyBase;
class UTTAnimNotify;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UTurnToAshesAbility : public UPlayAnimationAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	TObjectPtr<UTTAnimNotify> AnimNotify;

	TObjectPtr<AEnemyBase> Enemy;
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
	UFUNCTION()
	void ExecuteTurnToAshes();
};

// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PetrificationAbility.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UPetrificationAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	UPROPERTY(EditAnywhere, Category = "TT| Petrification Ability Info")
	TSubclassOf<UGameplayEffect> PetrificationEffectClass;
	UPROPERTY(EditAnywhere, Category = "TT| Petrification Ability Info")
	TObjectPtr<UMaterialInterface> PetrifiedMaterial;

	TObjectPtr<AEnemyBase> Enemy;
	float PetrificationDuration;
	
	UFUNCTION()
	void ApplyPetrification(const FGameplayTag Tag, int32 Value);
	UFUNCTION()
	void FinishPassive(const FGameplayTag Tag, int32 Value);
	
};

// TinyTavern

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "BurnEnemy.generated.h"

class AEnemyBase;
class UTTAnimNotify;
class UNiagaraSystem;
class UTurnToAshesTask;

/**
 * 
 */
UCLASS()
class TINYTAVERNGAME_API UBurnEnemy : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	TObjectPtr<AEnemyBase> Enemy;
	TObjectPtr<UMaterialInterface> BurnMaterial ;
protected:

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
	
	UPROPERTY()
	TObjectPtr<UTurnToAshesTask> TurnToAshesTask;


	UAnimMontage* AnimMontage;
private:
	
	TObjectPtr<UTTAnimNotify> AnimNotify;
	
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
	UFUNCTION()
	void ExecuteTurnToAshes();

	UFUNCTION()
	void EndLogic();
};

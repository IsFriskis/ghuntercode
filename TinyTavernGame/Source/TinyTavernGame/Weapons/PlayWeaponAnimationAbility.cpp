// TinyTavern


#include "PlayWeaponAnimationAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UPlayWeaponAnimationAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//Remove binding
	if(MontageTask)
	{
		if(MontageTask.Get()->OnCompleted.IsBound())
		{
			MontageTask.Get()->OnCompleted.RemoveAll(this);
		}
	}
}

void UPlayWeaponAnimationAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UPlayWeaponAnimationAbility::PlayWeaponAnimation()
{
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "EnemyAnimationMontage", AnimMontage);
	MontageTask.Get()->OnCompleted.AddDynamic(this, &UPlayWeaponAnimationAbility::FinishPlayAnimation);
	MontageTask.Get()->ReadyForActivation();
}

void UPlayWeaponAnimationAbility::FinishPlayAnimation()
{
	MontageTask.Get()->OnCompleted.RemoveAll(this);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

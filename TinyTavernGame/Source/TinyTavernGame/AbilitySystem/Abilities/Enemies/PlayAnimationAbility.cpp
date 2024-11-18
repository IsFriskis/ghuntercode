// TinyTavern


#include "PlayAnimationAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/TT_GameplayTags.h"

void UPlayAnimationAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "EnemyAnimationMontage", AnimMontage);
	MontageTask->OnCompleted.AddDynamic(this, &UPlayAnimationAbility::FinishPlayAnimation);
	MontageTask->OnCancelled.AddDynamic(this, &UPlayAnimationAbility::FinishPlayAnimation);
	MontageTask->ReadyForActivation();
}

void UPlayAnimationAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
	if (ShouldDieAtEnd)
	{
		ActorInfo->AbilitySystemComponent->GetOwner()->Destroy();
	}
}

void UPlayAnimationAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	//ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
}

void UPlayAnimationAbility::FinishPlayAnimation()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

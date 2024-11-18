// TinyTavern


#include "RollDiceAbility.h"

#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void URollDiceAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		ABasicCharacter* Character = Cast<ABasicCharacter>(ActorInfo->AvatarActor);
		float Diceresult = FMath::RandRange(0,99);
		if (Diceresult < TruePercentage)
		{
			Character->GetAbilitySystemComponent()->AddLooseGameplayTag(TrueTag);
		}
		else
		{
			Character->GetAbilitySystemComponent()->AddLooseGameplayTag(FalseTag);
		}
		
	
}

void URollDiceAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URollDiceAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

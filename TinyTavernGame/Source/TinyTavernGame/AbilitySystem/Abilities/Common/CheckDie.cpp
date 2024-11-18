// TinyTavern


#include "CheckDie.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyGroup.h"

void UCheckDie::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	FGameplayEventData Payload;
	TObjectPtr<AEnemyBase> enemy = Cast<AEnemyBase>(ActorInfo->AvatarActor.Get());
	if (enemy.Get()->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Vulnerable"))))
	{
		enemy.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(TriggerEventData->TargetTags.Last());
		enemy.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("States.Dead")));

		//Apply Die Effect to reproduce an animation when the enemy dies
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTags::Get().States_Dead);
		enemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
		
		enemy->GetAbilitySystemComponent()->ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Combat_EnemyDeath);

		if(enemy->GetGroup() != nullptr)
		{
			AEnemyGroup* group = enemy->GetGroup();
			TArray<AEnemyBase*> EnemiesInGroup = group->GetEnemiesInGroup();
			
			for(AEnemyBase* EnemyInGroup : EnemiesInGroup)
			{
				if(EnemyInGroup)
				{
					EnemyInGroup->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_DesperatelyFlee);
				}
			}
		}
	}
	else 
	{
		// //Apply Stun Effect if not petrified
		// //TODO Check if in attack mode
		// FGameplayEffectContextHandle effectContextHandle = enemy->GetAbilitySystemComponent()->MakeEffectContext();
		// effectContextHandle.AddSourceObject(this);
		//
		// FGameplayEffectSpecHandle stunSpecHandle = enemy->GetAbilitySystemComponent()->MakeOutgoingSpec(StunEffect, 1, effectContextHandle);
		// UAbilitySystemBlueprintLibrary::SetDuration(stunSpecHandle, TriggerEventData->EventMagnitude);
		// enemy.Get()->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*stunSpecHandle.Data.Get());
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
}

void UCheckDie::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// TinyTavern

#include "TurnToAshesAbility.h"
#include "NiagaraFunctionLibrary.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

void UTurnToAshesAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Enemy = Cast<AEnemyBase>(ActorInfo->OwnerActor);
	ExplosionEffect = Enemy.Get()->ExplosionEffect;
	//AnimNotify = Cast<UTTAnimNotify>(AnimMontage->AnimNotifyTracks[0].Notifies[0]->Notify.Get());
	AnimNotify = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());

	if(AnimNotify)
	{
		AnimNotify.Get()->OnNotified.AddDynamic(this, &UTurnToAshesAbility::ExecuteTurnToAshes);
	}
}

void UTurnToAshesAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AnimNotify.Get()->OnNotified.RemoveAll(this);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTurnToAshesAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	AnimNotify.Get()->OnNotified.RemoveAll(this);
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UTurnToAshesAbility::ExecuteTurnToAshes()
{
	FGameplayEffectContextHandle EffectContext = Enemy->GetAbilitySystemComponent()->MakeEffectContext();
	K2_ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Combat_EnemyAshesDeath,EffectContext);
}

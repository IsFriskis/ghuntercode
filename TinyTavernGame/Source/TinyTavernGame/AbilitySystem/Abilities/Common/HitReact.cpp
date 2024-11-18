// TinyTavern


#include "HitReact.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Character/CombatInterface.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"


void UHitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	
	HitReactAnim = HitReactAnim->CreatePlayMontageAndWaitProxy(this, "HitReact", Player->GetHitReactMontage_Implementation());
	HitReactAnim.Get()->OnCompleted.AddDynamic(this, &UHitReact::OnAnimationEnd);

	UGameplayEffect* GameplayEffectInstance = NewObject<UGameplayEffect>(GetTransientPackage(), GameplayEffect);
	if(Player)
	{
		Player->GetAbilitySystemComponent()->ApplyGameplayEffectToTarget(GameplayEffectInstance, Player->GetAbilitySystemComponent());
	}
}

void UHitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if(Player)
	{
	//	Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_Invulnerable);
	}
}
void UHitReact::OnAnimationEnd()
{
	HitReactAnim.Get()->OnCompleted.RemoveDynamic(this, &UHitReact::OnAnimationEnd);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

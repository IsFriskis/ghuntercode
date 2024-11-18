// TinyTavern


#include "GrillAbility.h"
#include "GrillAbilityTask.h"
#include "../../../TinyTavernUtils.h"
#include "../../../TT_GameplayTags.h"
#include "../../../AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "../../../Player/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGrillAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UGrillAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked, 3);
	
	// Turn off VFX
	ActorInfo->AbilitySystemComponent->RemoveGameplayCue(FGameplayTags::Get().GameplayCue_Combat_GrillSound);
	ActorInfo->AbilitySystemComponent->RemoveGameplayCue(FGameplayTags::Get().GameplayCue_Combat_GrillVFX);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGrillAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGrillAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UGrillAbility::FinishAbility(FGameplayEventData Payload)
{
	// End ability
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

bool UGrillAbility::NoAmmunition(APlayerCharacter* _Player)
{
	Player = _Player;
	PlayerAttributes = Cast<UTT_AttributeSet>(Player->GetAttributeSet());

	float CurrentAmmo = PlayerAttributes.Get()->GetGrilledAmmo();

	if (CurrentAmmo <= 0)
	{
		Player->DisplayNotificationPopUp(FGameplayTags::Get().Notification_OutOfGrillAmmo);
		return true;
	}
	else
	{	
		PlayerAttributes->SetGrilledAmmo(CurrentAmmo - CostPerShot);
		if (CooldownEffect)
		{
			ApplyCooldownEffect(CooldownBetweenShots);
		}
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerSingleFire);
		Player.Get()->GetAbilitySystemComponent()->AddGameplayCue(FGameplayTags::Get().GameplayCue_Combat_GrillSound);
		Player.Get()->GetAbilitySystemComponent()->AddGameplayCue(FGameplayTags::Get().GameplayCue_Combat_GrillVFX);

		if (FiringAnimation)
		{
			UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "SingleShotAnimation", FiringAnimation, 1.0f, FName("Default"));
			MontageTask->ReadyForActivation();
		}
	}
	return false;
}

void UGrillAbility::ShowIndicator()
{
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(PrevisualizationRange);
}

void UGrillAbility::ApplyCooldownEffect(float Duration)
{
	TObjectPtr<UAbilitySystemComponent> PlayerASC = Player.Get()->GetAbilitySystemComponent();
	//Grab the cooldown effect passed by reference and apply it setting the duration
	FGameplayEffectContextHandle EffectContextHandle = PlayerASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle CooldownSpecHandle = PlayerASC.Get()->MakeOutgoingSpec(CooldownEffect, 1, EffectContextHandle);

	//The duration is equal to the recovery time based on the charge amount of the weapon
	UAbilitySystemBlueprintLibrary::SetDuration(CooldownSpecHandle, Duration);

	PlayerASC.Get()->ApplyGameplayEffectSpecToSelf(*CooldownSpecHandle.Data.Get());
}

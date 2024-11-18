// TinyTavern

#include "BoilAbility.h"
#include "BoilAbilityTask.h"
#include "../../../TinyTavernUtils.h"
#include "../../../TT_GameplayTags.h"
#include "../../../AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "../../../Player/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UBoilAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	PlayerAttributes = Cast<UTT_AttributeSet>(Player->GetAttributeSet());

	if (PlayerAttributes.Get()->GetBoiledAmmo() <= 0)
	{
		Player->DisplayNotificationPopUp(FGameplayTags::Get().Notification_OutOfBoilAmmo);
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
	else
	{
		//Subscribe when ability is finished by input
		WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTags::Get().Events_Weapon_FinishRangeWeaponAbility);
		WaitGameplayEvent.Get()->EventReceived.AddDynamic(this, &UBoilAbility::FinishAbility);
		WaitGameplayEvent.Get()->ReadyForActivation();

		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerSingleFire);
		ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerFiring);
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
		ActorInfo->AbilitySystemComponent->AddGameplayCue(FGameplayTags::Get().GameplayCue_Combat_BoilSound);
		ActorInfo->AbilitySystemComponent->AddGameplayCue(FGameplayTags::Get().GameplayCue_Combat_BoilVFX);
		
		FBoilParams BoilParams;
		BoilParams.OwningAbility = this;
		BoilParams.OwningActor = ActorInfo->AvatarActor.Get();
		BoilParams.BaseAngle = HitboxAngle;
		BoilParams.BaseWidth = BaseWidth;
		BoilParams.MinWidth = MinWidth;
		BoilParams.BaseRange = BaseRange;
		BoilParams.MaxRange = MaxRange;
		BoilParams.BaseDamage = BaseDamagePerHit;
		BoilParams.MaxDamage = MaxDamagePerHit;
		BoilParams.DamageEffect = DamageEffect;
		BoilParams.CollisionChannel = CollisionChannel;
		BoilParams.AttributeSet = PlayerAttributes;
		BoilParams.CostPerSecond = CostPerSecond;
		BoilParams.WineDamageMultiplier = WineDamageMultiplier;
		BoilParams.OnHitVFX = OnHitVFX.GetDefaultObject();

		if (TriggerEventData->EventMagnitude != 0)
		{
			BoilParams.ConcentrationRate = 1 / TimeToMaxConcentrationUpgraded;
		}
		else
		{
			BoilParams.ConcentrationRate = 1 / TimeToMaxConcentration;
		}

		//GetCurrentAbilitySpec()->InputID = 3;

		// Create Ability Task
		BoilAbilityTask = UBoilAbilityTask::ExecuteTask(BoilParams);
		BoilAbilityTask->OnOutOfAmmo.AddDynamic(this, &UBoilAbility::OnOutOfAmmo);
		BoilAbilityTask->ReadyForActivation();
		
		//Fire Animation
		Player.Get()->ExecuteFireAnimation();

		if (!VFXComponent)
		{
			TArray<AActor*> BoilCue;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("GameplayCueBoil"), BoilCue);
			if (BoilCue.Num() > 0)
			{
				VFXComponent = BoilCue[0]->GetComponentByClass<UNiagaraComponent>();
			}
		}
	}
}

void UBoilAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// End task
	if (BoilAbilityTask && !BoilAbilityTask->MarkedEndTask && !AbilityMarkedForDeath)
	{
		BoilAbilityTask->PrepareForDestroy();
		AbilityMarkedForDeath = true;

		// Turn off VFX
		ActorInfo->AbilitySystemComponent->RemoveGameplayCue(FGameplayTags::Get().GameplayCue_Combat_BoilSound);

		EndVFXSpawnRate();
	}

	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerFiring);

	Player.Get()->SetStrafe(false);
	Player.Get()->StopFireAnimation();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBoilAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	/*//Remove Binding
	if (WaitGameplayEvent)
	{
		if (WaitGameplayEvent.Get()->EventReceived.IsBound())
		{
			WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
		}
	}*/

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UBoilAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UBoilAbility::OnOutOfAmmo()
{
	Player->DisplayNotificationPopUp(FGameplayTags::Get().Notification_OutOfBoilAmmo);
	EndAbility(FGameplayAbilitySpecHandle(), GetCurrentActorInfo(), FGameplayAbilityActivationInfo(), false, false);
}

void UBoilAbility::EndVFXSpawnRate()
{
	for (int i = 0; i < 4; i++)
	{
		FString ParameterString = FString("Boil") + FString::FromInt(i) + FString("_SpawnRate");
		FName ParameterName(*ParameterString);
		VFXComponent->SetFloatParameter(ParameterName, 0);
	}
}

void UBoilAbility::FinishAbility(FGameplayEventData Payload)
{
	// End ability
	WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
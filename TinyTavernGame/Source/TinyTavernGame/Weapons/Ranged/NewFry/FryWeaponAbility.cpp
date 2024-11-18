// TinyTavern


#include "FryWeaponAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "FryHoldTask.h"
#include "FryNewProjectile.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Settings/TT_GameInstance.h"

void UFryWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetCurrentAbilitySpec()->InputID = 3;
	if(TriggerEventData->EventMagnitude != 0)
	{
		IsUpgraded = true;
	}
	PlayerRef = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	
	PlayerRef->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().Weapon_Range_Fry);
	WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTags::Get().Events_Weapon_FinishRangeWeaponAbility);
	WaitGameplayEvent.Get()->EventReceived.AddDynamic(this, &UFryWeaponAbility::OnInputReleased);
	WaitGameplayEvent.Get()->ReadyForActivation();
	
	UTT_AttributeSet* AttributeSet = Cast<UTT_AttributeSet>(PlayerRef->GetAttributeSet());

	float currentAmmo = AttributeSet->GetFriedAmmo();

	TArray<FFryWeaponTiersParams> NewWpnTier;
	for(FFryWeaponTiersParams Tier : FryWeaponTiers)
	{
		if(currentAmmo >= AmmoCost * Tier.CostMultiplier)
		{
			NewWpnTier.Add(Tier);
		}
		else
		{
			break;
		}
	}
	if(PlayerRef)
	{
		PlayerRef->WeaponIndicatorStaticMesh.Get()->SetRelativeRotation(FRotator(0, 90.0f, 0));	
	}
	
	if(currentAmmo >= AmmoCost * FryWeaponTiers[0].CostMultiplier)
	{
		ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(FGameplayTags::Get().States_PlayerFiring);
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
		
		FryWeaponTiers = NewWpnTier;
		
		FryHoldTask = UFryHoldTask::ExecuteFryHoldTask(this);
		FryHoldTask.Get()->ReadyForActivation();	
		AudioComponent = UGameplayStatics::SpawnSoundAttached(
		ChargeSound,                  // Reemplaza con tu SoundCue
		PlayerRef->GetRootComponent(),
		NAME_None,
	FVector::ZeroVector,
	EAttachLocation::KeepRelativeOffset,
	true);

		AudioComponent->Play();
	}
	else if(currentAmmo < AmmoCost * FryWeaponTiers[0].CostMultiplier)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
		
	}
}

void UFryWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked, 3);
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerFiring);
	PlayerRef->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Weapon_Range_Fry);
	if(AudioComponent)
	{
		AudioComponent->Stop();
	}
}


void UFryWeaponAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);

	if (WeaponExplosionIndicatorStaticMesh)
	{
		// Detach the component from its parent
		WeaponExplosionIndicatorStaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		// Destroy the component
		WeaponExplosionIndicatorStaticMesh->ConditionalBeginDestroy();

		// Clear the reference
		WeaponExplosionIndicatorStaticMesh = nullptr;
	}
	PlayerRef->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(nullptr);

	if(NewFryProjectile)
	{
		NewFryProjectile->Destroy();
	}
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTags::Get().States_PlayerFiring);
}

void UFryWeaponAbility::OnInputReleased(FGameplayEventData Payload)
{
	WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
	float holdBlendValue = 0.0f;
	float holdSpeedValue = 0.0f;
	float holdTimeAliveValue = 0.0f;
	if(FryHoldTask)
	{
		holdBlendValue = FryHoldTask->GetHoldTime();
		holdSpeedValue = FryHoldTask->GetHoldSpeedTime();
		holdTimeAliveValue = FryHoldTask->GetHoldTimeAlive();
		FryHoldTask->EndTask();
	}
	if (WeaponExplosionIndicatorStaticMesh)
	{
		// Detach the component from its parent
		WeaponExplosionIndicatorStaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		// Destroy the component
		WeaponExplosionIndicatorStaticMesh->ConditionalBeginDestroy();

		// Clear the reference
		WeaponExplosionIndicatorStaticMesh = nullptr;
	}
	PlayerRef->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(nullptr);
	
	
	SpawnLogic(holdBlendValue, holdSpeedValue, holdTimeAliveValue);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	
}

void UFryWeaponAbility::ApplyCooldownEffect(float Duration)
{
	TObjectPtr<UAbilitySystemComponent> PlayerASC = PlayerRef->GetAbilitySystemComponent();
	//Grab the cooldown effect passed by reference and apply it setting the duration
	FGameplayEffectContextHandle EffectContextHandle = PlayerASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle CooldownSpecHandle = PlayerASC.Get()->MakeOutgoingSpec(CooldownEffect, 1, EffectContextHandle);

	//CooldownSpecHandle.Data->SetDuration(Duration, false);

	//The duration is equal to the recovery time based on the charge amount of the weapon
	UAbilitySystemBlueprintLibrary::SetDuration(CooldownSpecHandle, Duration);

	PlayerASC.Get()->ApplyGameplayEffectSpecToSelf(*CooldownSpecHandle.Data.Get());
}

int32 UFryWeaponAbility::FetchAbilityTier()
{
	int32 ToReturn = -1;
	for(FFryWeaponTiersParams Tier : FryWeaponTiers)
	{
		if(!IsUpgraded)
		{
			if(Tier.TimeHoldToBeActivated <= TimeHoldingInput)
			{
				ToReturn++;
			}
			else
			{
				break;
			}
		}
		else
		{
			if(Tier.TimeHoldToBeActivatedWhenUpgraded <= TimeHoldingInput)
			{
				ToReturn++;
			}
			else
			{
				break;
			}
		}
	}
	return ToReturn;
}

void UFryWeaponAbility::SpawnLogic(float HoldValue, float SpeedValue, float TimeAlive)
{
	AActor* ownerActor = GetCurrentActorInfo()->AvatarActor.Get();
	FVector forwardVector = ownerActor->GetActorForwardVector();
	FVector spawnLocation = Cast<APlayerCharacter>(ownerActor)->WeaponSkeletalMesh->GetSocketLocation("FrySpawnPos");
	FRotator parentRotator = ownerActor->GetActorRotation();	
	FActorSpawnParameters spawnParams;
	

	int32 ActualTier = FetchAbilityTier();
	UTT_AttributeSet* AttributeSet = Cast<UTT_AttributeSet>(PlayerRef->GetAttributeSet());

	float currentAmmo = AttributeSet->GetFriedAmmo();
	if (currentAmmo > 0 && ActualTier >= 0)
	{

		AttributeSet->SetFriedAmmo(currentAmmo - AmmoCost * FryWeaponTiers[ActualTier].CostMultiplier);
		
		
		if(NewFryProjectile)
		{
			float Distance = (TimeAlive * SpeedValue) / 100;
			float ExplosionRadius = BaseExplosionRadius * HoldValue;
			NewFryProjectile->InitValues(
				HoldValue,
				TimeAlive,
				ExplosionRadius * 100,
				BaseDamage * FryWeaponTiers[ActualTier].DamageMultiplier,
				SpeedValue,
				forwardVector, FryGameplayEffect,
				WineDamageMultiplier, ExplosionEffect, ExplosionSound);

			FGameplayEffectContextHandle EffectContext = GetContextFromOwner(nullptr);
			K2_ExecuteGameplayCue(FGameplayTags::Get().GameplayCue_Combat_FryShoot, EffectContext);
			NewFryProjectile->StartMoving();
		}
		ApplyCooldownEffect(BaseCooldown);
	}
	else
	{
		NewFryProjectile->Destroy();	
	}

	PlayerRef->PlayAnimMontage(FiringAnimation);

	
}

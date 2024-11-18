// TinyTavern


#include "JamSlamAbility.h"

#include "AbilitySystemComponent.h"
#include "JamSlamAimTask.h"
#include "JamSlamArea.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"

void UJamSlamAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	GetCurrentAbilitySpec()->InputID = 1;
	
	Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	//Check if Ammo > 0
	TObjectPtr<UTT_AttributeSet> AttributeSet = Cast<UTT_AttributeSet>(Player.Get()->GetAttributeSet());
	if(!AttributeSet || AttributeSet.Get()->GetJamAmmo() <= 0)
	{
		Player->DisplayNotificationPopUp(FGameplayTags::Get().Notification_OutOfJamAmmo);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	else
	{
		//Subscribe when ability is finished by input
		WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTags::Get().Events_Weapon_FinishIngredientWeaponAbility);
		WaitGameplayEvent.Get()->EventReceived.AddDynamic(this, &UJamSlamAbility::FinishAbility);
		WaitGameplayEvent.Get()->ReadyForActivation();
		//GetCurrentAbilitySpec()->InputID = 1;
		JamSlamAimTask = UJamSlamAimTask::ExecuteJamSlamAimTask(this, Player, PetrificationSpawnMaxRadius, PetrificationSpawnMinRadius, PetrificationAreaRadius, HoldAnim, JamIndicatorMesh, AimSmoothFactor);
		JamSlamAimTask.Get()->ReadyForActivation();
		//Add blocked_weapon Tag to Player
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
	}
}

void UJamSlamAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(false);
	//Remove binding

	if(JamAnimNotifySpawn)
	{
		JamAnimNotifySpawn.Get()->OnNotified.RemoveAll(this);
	}
	if(JamAnimNotify)
	{
		JamAnimNotify.Get()->OnNotified.RemoveAll(this);
	}

	//Remove blocked_weapon Tag to Player
	Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked,3);
	//Remove one to the jam ammo
	//Remove Mesh

	InputHasBeenReleased = false;
	bAlreadyEnding = false;
}

void UJamSlamAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	Player.Get()->StopAnimMontage();
	// //Remove binding
	// if(JamAnimNotifySpawn)
	// {
	// 	JamAnimNotifySpawn.Get()->OnNotified.RemoveAll(this);
	// }
	// if(JamAnimNotify)
	// {
	// 	JamAnimNotify.Get()->OnNotified.RemoveAll(this);
	// }
	//Remove blocked_weapon Tag to Player
	// Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
	if(WaitGameplayEvent)
	{
		WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
	}
}

void UJamSlamAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if(!bAlreadyEnding)
	{
		bAlreadyEnding = true;
		// Launch Jam
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(false);
		WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
		JamSlamAimTask.Get()->EndTask();
		PlayWeaponAnimation();
		JamSlamLogic();	
	}
	
}

void UJamSlamAbility::JamSlamLogic()
{
	//Play Jam Slam Animation, and on a notify, SpawnJam()
	//JamAnimNotify = Cast<UTTAnimNotify>(AnimMontage->AnimNotifyTracks[0].Notifies[0]->Notify.Get());
	JamAnimNotifySpawn = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());
	if(JamAnimNotifySpawn)
	{
		JamAnimNotifySpawn.Get()->OnNotified.AddDynamic(this, &UJamSlamAbility::SpawnJam);
	}
	JamAnimNotify = Cast<UTTAnimNotify>(AnimMontage->Notifies[1].Notify.Get());
	if(JamAnimNotify)
	{
		JamAnimNotify.Get()->OnNotified.AddDynamic(this, &UJamSlamAbility::ClearWeaponsOnNotify);
	}
}
void UJamSlamAbility::ClearWeaponsOnNotify()
{
	JamAnimNotify.Get()->OnNotified.RemoveDynamic(this, &UJamSlamAbility::ClearWeaponsOnNotify);
	Player.Get()->GetWeaponComponent()->ClearWeapons();
}
void UJamSlamAbility::FinishPlayAnimation()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UJamSlamAbility::SpawnJam()
{
	JamAnimNotifySpawn.Get()->OnNotified.RemoveDynamic(this, &UJamSlamAbility::SpawnJam);
	//Spawn Jam
	float Distance = FVector::Dist(Cast<ATTPlayerController>(Player.Get()->GetController())->GetMouseProjectionToWorld(), Player->GetActorLocation());
	Distance = FMath::Clamp(Distance, PetrificationSpawnMinRadius, PetrificationSpawnMaxRadius);
	FVector actor = Player.Get()->GetActorLocation();
	FVector forward = Player.Get()->GetActorForwardVector() * Distance;
	FVector petrificationAreaLocation = actor + forward;
	FVector projectileLocation = FVector(JamSpawnLocation.X ,
									JamSpawnLocation.Y,
									JamSpawnLocation.Z + HeightOfJamSlamMeteor);
	TObjectPtr<AJamSlamArea> JamSlamActor =	GetWorld()->SpawnActor<AJamSlamArea>(JamSlamAreaClass,
														projectileLocation,
														FRotator(0,0,0));
	JamSlamActor.Get()->SetPetrificationDuration(PetrificationDuration);
	JamSlamActor.Get()->SetPetrificationEffect(PetrificationEffect);
	JamSlamActor.Get()->SetPetrificationAreaRadius(PetrificationAreaRadius);
	JamSlamActor.Get()->SetPetrificationAreaTime(PetrificationAreaTime);
	JamSlamActor.Get()->SetInProcessOfPetrificationEffect(InProcessOfPetrificationEffect);
	JamSlamActor.Get()->SetJamSlamMeteorSpeed(JamSlamMeteorSpeed);
	JamSlamActor.Get()->SetTimeBeforePetrification(TimeBeforePetrification);
	JamSlamActor.Get()->SetPetrificationDurationAfterWine(PetrificationDurationAfterWine);
	JamSlamActor.Get()->SetExplosionVFX(SplashNiagaraSystem);
	

	TObjectPtr<UTT_AttributeSet> attributeSet = Cast<UTT_AttributeSet>(Player.Get()->GetAttributeSet());
	if(attributeSet)
	{
		attributeSet.Get()->SetJamAmmo(attributeSet.Get()->GetJamAmmo() - 1);
	}
}

void UJamSlamAbility::FinishAbility(FGameplayEventData Payload)
{
	if(!Player.Get()->GetWeaponComponent()->bIsIngredientActive)
	{
		// Launch Jam
		Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(false);
		WaitGameplayEvent.Get()->EventReceived.RemoveAll(this);
		JamSlamAimTask.Get()->EndTask();
		PlayWeaponAnimation();
		Player.Get()->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTags::Get().States_WeaponsBlocked);
		Player.Get()->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTags::Get().Events_Weapon_RangedAiming);
		JamSlamLogic();
	}
	else
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

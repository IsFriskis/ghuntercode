// TinyTavern


#include "AimingRangedAbility.h"

#include "AimingRangeAbilityTask.h"
#include "TinyTavernGame/TT_GameplayTags.h"
#include "TinyTavernGame/Components/WeaponComponent.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

void UAimingRangedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	AimRangeTask = UAimingRangeAbilityTask::ExecuteAimRangeTask(this, Player, GrillIndicatorMesh, BoilIndicatorMesh,
																FryIndicatorMesh, FryExplosionIndicatorMesh, IndicatorsInfo);
	AimRangeTask.Get()->ReadyForActivation();
}

void UAimingRangedAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(false);
	Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetVisibility(false);
	// Player.Get()->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(nullptr);
	// Player.Get()->WeaponIndicatorStaticMesh2.Get()->SetStaticMesh(nullptr);
}

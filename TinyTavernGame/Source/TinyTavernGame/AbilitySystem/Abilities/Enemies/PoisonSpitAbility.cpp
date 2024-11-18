// TinyTavern


#include "PoisonSpitAbility.h"

#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Character/TTAnimNotify.h"
#include "TinyTavernGame/Character/Enemy/Basilisco/PoisonSpitProjectile.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

void UPoisonSpitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Enemy = Cast<AEnemyBase>(ActorInfo->OwnerActor);
	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PoisonAnimNotify = Cast<UTTAnimNotify>(AnimMontage->Notifies[0].Notify.Get());
	if(PoisonAnimNotify)
	{
		PoisonAnimNotify.Get()->OnNotified.AddDynamic(this, &UPoisonSpitAbility::ExecuteSpit);
	}
}

void UPoisonSpitAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	//Remove binding
	PoisonAnimNotify.Get()->OnNotified.RemoveAll(this);
}

void UPoisonSpitAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	//Remove binding
	PoisonAnimNotify.Get()->OnNotified.RemoveAll(this);
}

void UPoisonSpitAbility::ExecuteSpit()
{
	FVector SpitSpawnLocation = Enemy->GetMesh()->GetSocketLocation(FName("Mouth"));
	FVector PlayerSocketLocation = Player->GetMesh()->GetSocketLocation(FName("PlayerLocation"));
	
	FVector projectileDirection = (PlayerSocketLocation-SpitSpawnLocation).GetSafeNormal();
	TObjectPtr<APoisonSpitProjectile> PoisonSpitActor =	GetWorld()->SpawnActor<APoisonSpitProjectile>(PoisonSpitProjectileClass, SpitSpawnLocation, projectileDirection.Rotation());
	PoisonSpitActor.Get()->SetMaxDistance(ProjectileMaxDistance);
	PoisonSpitActor.Get()->SetProjectileSpeed(ProjectileSpeed);
	PoisonSpitActor.Get()->SetProjectileDirection(projectileDirection);
	PoisonSpitActor.Get()->SetProjectileRadius(ProjectileRadius);
	PoisonSpitActor.Get()->SetProjectileExplosionRadius(ProjectileExplosionRadius);
	PoisonSpitActor.Get()->SetDamageEffect(DamageEffect);
}

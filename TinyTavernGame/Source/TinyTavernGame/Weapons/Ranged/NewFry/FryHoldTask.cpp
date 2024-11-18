// TinyTavern


#include "FryHoldTask.h"

#include "FryNewProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/StaticMeshSocket.h"
#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"

UFryHoldTask::UFryHoldTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UFryHoldTask> UFryHoldTask::ExecuteFryHoldTask(UGameplayAbility* OwningAbility)
{
	UFryHoldTask* FryHoldTask = NewAbilityTask<UFryHoldTask>(OwningAbility);
	 
	FryHoldTask->FryWeaponAbility = Cast<UFryWeaponAbility>(OwningAbility);
	FryHoldTask->Timer = 0.0f;
	FryHoldTask->MaxTierOfFry = FryHoldTask->FryWeaponAbility->FryWeaponTiers.Num()-1;
	if (FryHoldTask->FryWeaponAbility.Get()->GetIsUpgraded())
	{
		FryHoldTask->FryProjectileBlend.SetBlendTime(FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].TimeHoldToBeActivatedWhenUpgraded);
		FryHoldTask->FryProjectileSpeedBlend.SetBlendTime(FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].TimeHoldToBeActivatedWhenUpgraded);
		FryHoldTask->FryProjectileTimeAliveBlend.SetBlendTime(FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].TimeHoldToBeActivatedWhenUpgraded);
	}
	else
	{
		FryHoldTask->FryProjectileBlend.SetBlendTime(FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].TimeHoldToBeActivated);
		FryHoldTask->FryProjectileSpeedBlend.SetBlendTime(FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].TimeHoldToBeActivated);
		FryHoldTask->FryProjectileTimeAliveBlend.SetBlendTime(FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].TimeHoldToBeActivated);
	}
	FryHoldTask->FryProjectileBlend.SetValueRange(FryHoldTask->FryWeaponAbility->BaseSizeScale, FryHoldTask->FryWeaponAbility->BaseSizeScale * FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].SizeScaleMultiplier);
	FryHoldTask->FryProjectileSpeedBlend.SetValueRange(FryHoldTask->FryWeaponAbility->BaseSpeed, FryHoldTask->FryWeaponAbility->BaseSpeed * FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].SpeedMultiplier);
	FryHoldTask->FryProjectileTimeAliveBlend.SetValueRange(FryHoldTask->FryWeaponAbility->BaseTimeAlive, FryHoldTask->FryWeaponAbility->BaseTimeAlive * FryHoldTask->FryWeaponAbility->FryWeaponTiers[FryHoldTask->MaxTierOfFry].TimeAliveMultiplier);
	

	return FryHoldTask;
}
void UFryHoldTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	TimerLogic(DeltaTime);
	
	if(FirstExec)
	{
		FryWeaponAbility->PlayerRef->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(FryWeaponAbility->PrevisualizationRange);
		
		if(UStaticMeshSocket* Socket = FryWeaponAbility->PlayerRef->WeaponIndicatorStaticMesh.Get()->GetStaticMesh()->FindSocket("ExplosionSocket"))
		{
			// Create a new static mesh component
			UStaticMeshComponent* NewMeshComponent = NewObject<UStaticMeshComponent>(FryWeaponAbility->PlayerRef);

			// Attach the new component to the socket
			NewMeshComponent->AttachToComponent(FryWeaponAbility->PlayerRef->WeaponIndicatorStaticMesh.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket->SocketName);

			// Set the static mesh for the new component
			NewMeshComponent->SetStaticMesh(FryWeaponAbility->PrevisualizationExplossion);

			// Register the new component
			NewMeshComponent->RegisterComponent();
			FryWeaponAbility->WeaponExplosionIndicatorStaticMesh = NewMeshComponent;
		}
		FirstExec = false;


		AActor* ownerActor = FryWeaponAbility->GetCurrentActorInfo()->AvatarActor.Get();
		FVector forwardVector = ownerActor->GetActorForwardVector();
		FVector spawnLocation = Cast<APlayerCharacter>(ownerActor)->WeaponSkeletalMesh->GetSocketLocation("FrySpawnPos");
		FRotator parentRotator = ownerActor->GetActorRotation();	
		FActorSpawnParameters spawnParams;
		FryWeaponAbility->NewFryProjectile = GetWorld()->SpawnActor<AFryNewProjectile>( FryWeaponAbility->FryProjectile, spawnLocation, parentRotator, spawnParams);
	}
 
	FryWeaponAbility->NewFryProjectile->SetActorLocation(FryWeaponAbility->PlayerRef->WeaponSkeletalMesh.Get()->GetSocketLocation("FrySpawnPos"));
	FryWeaponAbility->NewFryProjectile->SetActorRotation(FryWeaponAbility->PlayerRef->GetActorRotation());
	
	//int32 Tier = FryWeaponAbility->FetchAbilityTier();

	// if(Tier != -1)
	// {
	// 	PrevisualizationLogicWithTiers(Tier);
	// 	FryWeaponAbility->NewFryProjectile->UpdateScale(FryWeaponAbility->BaseSizeScale * FryWeaponAbility->FryWeaponTiers[Tier].SizeScaleMultiplier);
	// 	FryWeaponAbility->NewFryProjectile->SetNiagaraRotationSpeed(FryWeaponAbility->BaseRotationSpeed * FryWeaponAbility->FryWeaponTiers[Tier].RotationSpeedMultiplier);
	// 	FryWeaponAbility->NewFryProjectile->SetDistorsionBallLevel(FryWeaponAbility->DistorsionBallLevel * FryWeaponAbility->FryWeaponTiers[Tier].DistorsionBallLevelMultiplier);
	// }
	// else
	// {
	// 	FryWeaponAbility->WeaponExplosionIndicatorStaticMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	// }
	FryWeaponAbility->PlayerRef->WeaponIndicatorStaticMesh.Get()->SetVisibility(true);
	FryProjectileBlend.Update(DeltaTime);
	FryProjectileSpeedBlend.Update(DeltaTime);
	FryProjectileTimeAliveBlend.Update(DeltaTime);
	FryWeaponAbility->NewFryProjectile->UpdateScale(FryWeaponAbility->BaseSizeScale * FryProjectileBlend.GetBlendedValue());
	FryWeaponAbility->NewFryProjectile->SetNiagaraRotationSpeed(FryWeaponAbility->BaseRotationSpeed * FryProjectileBlend.GetBlendedValue() * FryWeaponAbility->FryWeaponTiers[MaxTierOfFry].RotationSpeedMultiplier);
	FryWeaponAbility->NewFryProjectile->SetDistorsionBallLevel(FryWeaponAbility->DistorsionBallLevel * FryProjectileBlend.GetBlendedValue() * FryWeaponAbility->FryWeaponTiers[MaxTierOfFry].DistorsionBallLevelMultiplier);
	float Distance = (FryProjectileTimeAliveBlend.GetBlendedValue() * FryProjectileSpeedBlend.GetBlendedValue() + 200) / 100;
	FryWeaponAbility->PlayerRef->WeaponIndicatorStaticMesh.Get()->SetWorldScale3D(FVector(1.0f, Distance, 1.0f));
	FryWeaponAbility->PlayerRef->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(Distance * 50 * FryWeaponAbility->PlayerRef->GetActorForwardVector() + FryWeaponAbility->PlayerRef->GetActorLocation());
	float ExplosionRadius = FryWeaponAbility->BaseExplosionRadius * FryProjectileBlend.GetBlendedValue();
	FryWeaponAbility->WeaponExplosionIndicatorStaticMesh->SetWorldScale3D(FVector(ExplosionRadius, ExplosionRadius, 1.0f));
	printVector(FVector(ExplosionRadius, ExplosionRadius, 1.0f));
}

void UFryHoldTask::TimerLogic(float DeltaTime)
{
	Timer += DeltaTime;
	FryWeaponAbility->SetHoldTime(Timer);
}

void UFryHoldTask::PrevisualizationLogicWithTiers(int32 Tier)
{
	float ExplosionRadius = FryWeaponAbility->BaseExplosionRadius * FryWeaponAbility->FryWeaponTiers[Tier].ExplosionRadiusMultiplier / 100;
	float TimeAlive = FryWeaponAbility->BaseTimeAlive * FryWeaponAbility->FryWeaponTiers[Tier].TimeAliveMultiplier;
	float Speed = FryWeaponAbility->BaseSpeed * FryWeaponAbility->FryWeaponTiers[Tier].SpeedMultiplier;
	float Distance = (TimeAlive * Speed + 200) / 100;
	
	FryWeaponAbility->PlayerRef->WeaponIndicatorStaticMesh.Get()->SetWorldScale3D(FVector(1.0f, Distance , 1.0f));
		
	FryWeaponAbility->PlayerRef->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(((Distance * 50)) * FryWeaponAbility->PlayerRef->GetActorForwardVector() + FryWeaponAbility->PlayerRef->GetActorLocation());
		
	FryWeaponAbility->WeaponExplosionIndicatorStaticMesh->SetWorldScale3D(FVector(ExplosionRadius, ExplosionRadius, 1.0f));
}
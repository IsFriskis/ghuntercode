// TinyTavern

#include "GrillAbilityTask.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../../../TT_GameplayTags.h"
#include "Components/MeshComponent.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"
#include "../../../TinyTavernUtils.h"
#include "Components/CapsuleComponent.h"


UGrillAbilityTask::UGrillAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UGrillAbilityTask> UGrillAbilityTask::ExecuteTask(FGrillParams Params)
{
	TObjectPtr<UGrillAbilityTask> GrillTask = NewAbilityTask<UGrillAbilityTask>(Params.OwningAbility);
	GrillTask->Params = Params;
	
	GrillTask->WeaponMesh = Cast<APlayerCharacter>(Params.OwningActor)->WeaponSkeletalMesh;

	return GrillTask;
}


void UGrillAbilityTask::TickTask(float DeltaTime)
{
	if (MarkedEndTask)
	{
		EndTask();
		return;
	}

	// Ammo check
	float currentAmmo = Params.AttributeSet.Get()->GetGrilledAmmo();
	if (currentAmmo > 0)
	{
		Params.AttributeSet.Get()->SetGrilledAmmo(currentAmmo - (Params.CostPerSecond * DeltaTime));
		
		SocketLocation = WeaponMesh->GetSocketLocation("Muzzle");
		// SweepMultiByChannel with box in front of player at max range
		TArray<FHitResult> hits;

		ShowHitbox();
		if (IsInsideBoxSweep(hits))
		{
			for (const FHitResult& hit : hits)
			{
				// Limit by Range
				if (FVector::Distance(SocketLocation, hit.GetActor()->GetActorLocation()) <= Params.Range)
				{
					// Limit hits by angle & blocking
					if (IsNotBlocked(hit) && IsInsideAngle(hit))
					{
						// Apply gameplay effect to enemy
						if (TObjectPtr<UAbilitySystemComponent> targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(hit.GetActor()))
						{
							FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
							effectContextHandle.AddSourceObject(this);

							if (targetASC->HasMatchingGameplayTag(FGameplayTags::Get().States_Drunk))
							{
								ApplyEffect(Params.DamageEffect, Params.WeaponDamagePerSecond * Params.DrunkDamageMultiplier * DeltaTime, hit.GetActor(), targetASC, effectContextHandle);
								//ApplyEffect(Params.BurnEffect, Params.BurnDamagePerTick * Params.DrunkDamageMultiplier, hit.GetActor(), targetASC, effectContextHandle);
							}
							else
							{
								if (targetASC->HasMatchingGameplayTag(FGameplayTags::Get().States_Chase))
								{
									targetASC->RemoveLooseGameplayTag(FGameplayTags::Get().States_Chase);
								}
								ApplyEffect(Params.DamageEffect, Params.WeaponDamagePerSecond * DeltaTime, hit.GetActor(), targetASC, effectContextHandle);

								//ApplyEffect(Params.BurnEffect, Params.BurnDamagePerTick, hit.GetActor(), targetASC, effectContextHandle);
							}
						}
					}
				}
			}
		}
		NewEnemyInDamage(hits);

	}
	else
	{
		OnOutOfAmmo.Broadcast();
	}

	
}

void UGrillAbilityTask::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
	TArray<FHitResult> emptyHits;
	NewEnemyInDamage(emptyHits);
}


void UGrillAbilityTask::ShowHitbox()
{
	// Get player rotation z in degrees
	FRotator parentRotation = Params.OwningActor->GetActorRotation();
	float ZRot = parentRotation.Euler().Z;

	// Get player sides to get offset
	FVector ActorSidesOffsetDirection = Params.OwningActor->GetActorRightVector();
	FVector ActorForwardOffsetDirection = Params.OwningActor->GetActorForwardVector();
	FVector ActorSideOffset = ActorSidesOffsetDirection * Params.Width / 2;

	// Convert to radians and account for weapon max angle
	float angleToDraw = FMath::DegreesToRadians(ZRot + Params.Angle / 2);
	float angleToDrawNeg = FMath::DegreesToRadians(ZRot - Params.Angle / 2);

	// Line vectors
	FVector debugLineEnd = (SocketLocation + ActorSideOffset) + FVector(FMath::Cos(angleToDraw), FMath::Sin(angleToDraw), 0) * Params.Range;
	FVector debugLineEndNeg = (SocketLocation - ActorSideOffset) + FVector(FMath::Cos(angleToDrawNeg), FMath::Sin(angleToDrawNeg), 0) * Params.Range;

	// Draw lines
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), SocketLocation + ActorSideOffset, debugLineEnd, FColor(255, 0, 0), false, -1, 0, 12.333);
	DrawDebugLine(GetWorld(), SocketLocation - ActorSideOffset, debugLineEndNeg, FColor(255, 0, 0), false, -1, 0, 12.333);
#endif
}

void UGrillAbilityTask::ApplyEffect(TSubclassOf<UGameplayEffect> Effect, float Magnitude, AActor* Actor, UAbilitySystemComponent* TargetASC, FGameplayEffectContextHandle EffectContextHandle)
{
	FGameplayEffectSpecHandle effectSpecHandle = TargetASC->MakeOutgoingSpec(Effect, 1, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(effectSpecHandle, FGameplayTags::Get().States_Cook_Grilled, Magnitude);
	TargetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());
}

bool UGrillAbilityTask::IsInsideBoxSweep(TArray<FHitResult>& Hits)
{
	FVector boxLocation = SocketLocation + (Params.OwningActor->GetActorForwardVector() * Params.Range);
	FCollisionShape collisionShape;
	FQuat playerQuaternion = Params.OwningActor->GetActorRotation().Quaternion();

	/* Box is big so it can detect all objects no matter the angle 
	- This is fine because the angle will be filtered out later */

	FVector3f boxExtents = FVector3f(Params.Range, Params.Range, 100.0f);
	collisionShape.SetBox(boxExtents);
	FCollisionQueryParams params;
	params.AddIgnoredActor(Params.OwningActor);
	bool isHit = GetWorld()->SweepMultiByChannel(Hits, boxLocation, boxLocation, playerQuaternion, Params.CollisionChannel, collisionShape, params);

	// Display Sweep box
#if WITH_EDITOR
	//DrawDebugBox(GetWorld(), boxLocation, static_cast<FVector>(boxExtents), playerQuaternion, FColor::Emerald, false, -1, 0, 5);
#endif
	return isHit;
}

bool UGrillAbilityTask::IsInsideAngle(const FHitResult& Hit)
{
	float halfWidth = Params.Width / 2;

	FVector enemyPosition = Hit.GetActor()->GetActorLocation();

	FVector playerRightVector = Params.OwningActor->GetActorRightVector();

	FVector leftPlayerEdge = SocketLocation - (playerRightVector * halfWidth);
	FVector rightPlayerEdge = SocketLocation + (playerRightVector * halfWidth);

	FVector hitToPlayerLeft = enemyPosition - leftPlayerEdge;
	FVector hitToPlayerRight = enemyPosition - rightPlayerEdge;

	hitToPlayerLeft.Z = 0;
	hitToPlayerLeft.Normalize();

	hitToPlayerRight.Z = 0;
	hitToPlayerRight.Normalize();

	FVector playerForwardVector = Params.OwningActor->GetActorForwardVector();

	float dotProductLeft = FVector::DotProduct(hitToPlayerLeft, playerForwardVector);
	FVector crossProductLeft = FVector::CrossProduct(playerForwardVector, hitToPlayerLeft);
	float leftAngleRadians = FMath::Acos(dotProductLeft);
	float leftAngleDegrees = FMath::RadiansToDegrees(leftAngleRadians);
	if (crossProductLeft.Z < 0)
	{
		leftAngleDegrees = -leftAngleDegrees;
	}


	float dotProductRight = FVector::DotProduct(hitToPlayerRight, playerForwardVector);
	FVector crossProductRight = FVector::CrossProduct(playerForwardVector, hitToPlayerRight);
	float rightAngleRadians = FMath::Acos(dotProductRight);
	float rightAngleDegrees = FMath::RadiansToDegrees(rightAngleRadians);
	if (crossProductRight.Z < 0)
	{
		rightAngleDegrees = -rightAngleDegrees;
	}


	float halfAngle = Params.Angle / 2;

	if (leftAngleDegrees >= -halfAngle && rightAngleDegrees <= halfAngle)
	{
		return true;
	}
	else
	{
		return false;
	}


}

bool UGrillAbilityTask::IsNotBlocked(const FHitResult& Hit)
{
	FVector hitToPlayer = Hit.GetActor()->GetActorLocation() - SocketLocation;
	hitToPlayer.Normalize();

	FHitResult blockingHit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(Params.OwningActor);
	GetWorld()->LineTraceSingleByChannel(blockingHit, SocketLocation, SocketLocation + hitToPlayer * Params.Range, Params.CollisionChannel, params);
#if WITH_EDITOR
	//DrawDebugLine(GetWorld(), SocketLocation, SocketLocation + hitToPlayer * WeaponMaxRangeY, FColor(255, 0, 0), false, -1, 0, 12.333);
#endif
	return blockingHit.GetActor() == Hit.GetActor();
}

void UGrillAbilityTask::NewEnemyInDamage(TArray<FHitResult> Hits)
{
	// //Get a copy of the enemies
	// for(FHitResult hit : Hits)
	// {
	// 	if(!PreviousEnemiesInDamage.Contains(hit.GetActor()))
	// 	{
	// 		PreviousEnemiesInDamage.Add(hit.GetActor());
	// 	}
	// }
	//
	// for (TObjectPtr<AActor> previous : PreviousEnemiesInDamage)
	// {
	// 	TObjectPtr<UAbilitySystemComponent> targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Cast<AEnemyBase>(previous));
	// 	if(Hits.IsValidIndex(0))
	// 	{
	// 		for(FHitResult hit : Hits)
	// 		{
	// 			if(hit.GetActor()->IsValidLowLevel() && hit.GetActor() == previous)
	// 			{
	// 				break;
	// 			}
	// 			if(targetASC)
	// 			{
	// 				FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
	// 				effectContextHandle.AddSourceObject(this);
	// 				ApplyEffect(Params.BurnEffect, Params.BurnDamagePerTick, previous, targetASC, effectContextHandle);
	// 			}
	// 			PreviousEnemiesInDamage.Remove(previous);
	// 		}
	// 	}
	// 	else if(targetASC)
	// 	{
	// 		FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
	// 		effectContextHandle.AddSourceObject(this);
	// 		ApplyEffect(Params.BurnEffect, Params.BurnDamagePerTick, previous, targetASC, effectContextHandle);
	// 		PreviousEnemiesInDamage.Remove(previous);
	// 	}
	// 	else
	// 	{
	// 		PreviousEnemiesInDamage.Remove(previous);
	// 	}
	// }
}

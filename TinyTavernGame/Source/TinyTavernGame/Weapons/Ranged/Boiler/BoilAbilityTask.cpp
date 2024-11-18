// TinyTavern

#include "BoilAbilityTask.h"
#include "BoilAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../../../TT_GameplayTags.h"
#include "../../../Player/PlayerCharacter.h"
#include "Components/MeshComponent.h"
#include "../../../AbilitySystem/AttributeSet/TT_AttributeSet.h"
#include "../../../TinyTavernUtils.h"
#include "NiagaraComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "TinyTavernGame/Character/Enemy/EnemyBaseClass/EnemyBase.h"

UBoilAbilityTask::UBoilAbilityTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

TObjectPtr<UBoilAbilityTask> UBoilAbilityTask::ExecuteTask(FBoilParams Params)
{
	TObjectPtr<UBoilAbilityTask> BoilTask = NewAbilityTask<UBoilAbilityTask>(Params.OwningAbility);
	BoilTask->Params = Params;
	BoilTask->CurrentAngle = Params.BaseAngle;
	BoilTask->CurrentDamage = Params.BaseDamage;
	BoilTask->CurrentWidth = Params.BaseWidth;
	BoilTask->CurrentRange = Params.BaseRange;
	BoilTask->WineDamageMultiplier = Params.WineDamageMultiplier;
	BoilTask->CurrentConcentration = 0;
	BoilTask->OnHitVFX = Params.OnHitVFX;

	BoilTask->Player = Cast<APlayerCharacter>(Params.OwningActor);
	
	TArray<AActor*> BoilCue;
	UGameplayStatics::GetAllActorsWithTag(BoilTask->GetWorld(), FName("GameplayCueBoil"), BoilCue);
	if (BoilCue.Num() > 0)
	{
		BoilTask->VFXComponent = BoilCue[0]->GetComponentByClass<UNiagaraComponent>();
	}

	return BoilTask;
}

void UBoilAbilityTask::PrepareForDestroy()
{
	if (Ability && OnOutOfAmmo.IsBound())
	{
		//print("Removing delegates for Ability");
		//OnOutOfAmmo.RemoveAll(Ability);
	}
	
	MarkedEndTask = true;
}

void UBoilAbilityTask::TickTask(float DeltaTime)
{
	if (!OnOutOfAmmo.IsBound() && MarkedEndTask)
	{
		EndTask();
	}

	// Ammo Check
	float currentAmmo = Params.AttributeSet.Get()->GetBoiledAmmo();
	if (currentAmmo > 0)
	{
		Params.AttributeSet.Get()->SetBoiledAmmo(currentAmmo - (Params.CostPerSecond * DeltaTime));

		TArray<FHitResult> BoxHits;

		ShowHitbox();
		if (IsInsideBoxSweep(BoxHits))
		{
			// Limit hits by angle & blocking
			FilterByAngle(BoxHits);
			HandleEdgeCases();

			for (AActor* Actor : ConfirmedHits)
			{
				// Apply gameplay effect to enemy
				if (AEnemyBase* Enemy = Cast<AEnemyBase>(Actor))
				{
					TObjectPtr<UAbilitySystemComponent> targetASC = Enemy->GetAbilitySystemComponent();
					FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
					effectContextHandle.AddSourceObject(this);
					bool IsDrunk = targetASC->HasMatchingGameplayTag(FGameplayTags::Get().States_Drunk);
					float DamageToApply = IsDrunk ? CurrentDamage * WineDamageMultiplier : CurrentDamage;
					ApplyEffect(Params.DamageEffect, DamageToApply * DeltaTime, Enemy, targetASC, effectContextHandle);
				 	Enemy->GetAbilitySystemComponent()->ApplyGameplayEffectToTarget(OnHitVFX, Enemy->GetAbilitySystemComponent());
				}
			}	
		}
		ConfirmedHits.Empty();
		EdgeCases.Empty();
		CurrentConcentration += Params.ConcentrationRate * DeltaTime;
		CurrentConcentration = FMath::Clamp(CurrentConcentration, 0, 1);
		UpdateAttributes();
		UpdateVFXParameters();
	}
	else
	{
		OnOutOfAmmo.Broadcast();
	}
}

void UBoilAbilityTask::ApplyEffect(TSubclassOf<UGameplayEffect> Effect, float Magnitude, AActor* Actor, UAbilitySystemComponent* TargetASC, FGameplayEffectContextHandle EffectContextHandle)
{
	FGameplayEffectSpecHandle effectSpecHandle = TargetASC->MakeOutgoingSpec(Effect, 1, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(effectSpecHandle, FGameplayTags::Get().States_Cook_Boiled, Magnitude);
	TargetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());
}

void UBoilAbilityTask::ShowHitbox()
{
	// Get player values
	FVector parentLocation = Params.OwningActor->GetActorLocation();
	FRotator parentRotation = Params.OwningActor->GetActorRotation();
	float ZRot = parentRotation.Euler().Z;

	// Get player sides to get offset
	FVector ActorOffsetDirection = Params.OwningActor->GetActorRightVector();
	FVector ActorOffset = ActorOffsetDirection * CurrentWidth / 2;

	// Convert to radians and account for weapon max angle
	float angleToDraw = FMath::DegreesToRadians(ZRot + CurrentAngle / 2);
	float angleToDrawNeg = FMath::DegreesToRadians(ZRot - CurrentAngle / 2);
	// Line vectors
	FVector finalAngle = FVector(FMath::Cos(angleToDraw), FMath::Sin(angleToDraw), 0);
	FVector finalAngleNeg = FVector(FMath::Cos(angleToDrawNeg), FMath::Sin(angleToDrawNeg), 0);
	FVector debugLineEnd = (parentLocation + ActorOffset) + finalAngle * CurrentRange;
	FVector debugLineEndNeg = (parentLocation - ActorOffset) + finalAngleNeg * CurrentRange;

	// Draw lines
#if WITH_EDITOR
	// DrawDebugLine(GetWorld(), parentLocation + ActorOffset, debugLineEnd, FColor(255, 0, 0), false, -1, 0, 12.333);
	// DrawDebugLine(GetWorld(), parentLocation - ActorOffset, debugLineEndNeg, FColor(255, 0, 0), false, -1, 0, 12.333);
#endif
}

bool UBoilAbilityTask::IsInsideBoxSweep(TArray<FHitResult>& Hits)
{
	FVector boxLocation = Params.OwningActor->GetActorLocation() + (Params.OwningActor->GetActorForwardVector() * CurrentRange / 2);
	FCollisionShape collisionShape;
	FQuat playerQuaternion = Params.OwningActor->GetActorRotation().Quaternion();

	/* Box is big so it can detect all objects no matter the angle
	- This is fine because the angle will be filtered out later */

	FVector3f boxExtents = FVector3f(CurrentRange / 2, CurrentRange / 2, 100.0f);
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

void UBoilAbilityTask::FilterByAngle(TArray<FHitResult>& BoxHits)
{
	for (const FHitResult& Hit : BoxHits)
	{
		float halfWidth = CurrentWidth / 2;

		FVector enemyPosition = Hit.GetActor()->GetActorLocation();

		FVector playerRightVector = Params.OwningActor->GetActorRightVector();
		FVector playerPosition = Params.OwningActor->GetActorLocation();

		FVector leftPlayerEdge = playerPosition - (playerRightVector * halfWidth);
		FVector rightPlayerEdge = playerPosition + (playerRightVector * halfWidth);

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


		float halfAngle = CurrentAngle / 2;

		if (leftAngleDegrees >= -halfAngle && rightAngleDegrees <= halfAngle)
		{
			ConfirmedHits.Add(Hit.GetActor());
		}
		else
		{
			EdgeCases.Add(Hit.GetActor());
		}
	}
	
}

bool UBoilAbilityTask::IsNotBlocked(const FHitResult& Hit)
{
	FVector PlayerLocation = Params.OwningActor->GetActorLocation();
	FVector hitToPlayer = Hit.GetActor()->GetActorLocation() - PlayerLocation;
	hitToPlayer.Normalize();

	FHitResult blockingHit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(Params.OwningActor);
	GetWorld()->LineTraceSingleByChannel(blockingHit, PlayerLocation, PlayerLocation + hitToPlayer * CurrentRange, Params.CollisionChannel, params);
#if WITH_EDITOR
	//DrawDebugLine(GetWorld(), SocketLocation, SocketLocation + hitToPlayer * WeaponMaxRangeY, FColor(255, 0, 0), false, -1, 0, 12.333);
#endif
	return blockingHit.GetActor() == Hit.GetActor();
}

void UBoilAbilityTask::HandleEdgeCases()
{
	float HalfAngle = FMath::DegreesToRadians(CurrentAngle / 2.0f);
	float HalfWidth = CurrentWidth / 2;
	FVector ForwardVector = Params.OwningActor->GetActorForwardVector();
	FVector PlayerRightVector = Params.OwningActor->GetActorRightVector();


	// Rotate left side vector
	float LeftXComp = (cos(-HalfAngle) * ForwardVector.X) - (sin(-HalfAngle) * ForwardVector.Y);
	float LeftYComp = (sin(-HalfAngle) * ForwardVector.X) + (cos(-HalfAngle) * ForwardVector.Y);
	FVector LeftConeDir = FVector(LeftXComp, LeftYComp, 0);
	LeftConeDir.Normalize();
	FVector LeftPlayerEdge = Params.OwningActor->GetActorLocation() - (PlayerRightVector * HalfWidth);


	// Rotate right side vector
	float RightXComp = (cos(HalfAngle) * ForwardVector.X) - (sin(HalfAngle) * ForwardVector.Y);
	float RightYComp = (sin(HalfAngle) * ForwardVector.X) + (cos(HalfAngle) * ForwardVector.Y);
	FVector RightConeDir = FVector(RightXComp, RightYComp, 0);
	RightConeDir.Normalize();
	FVector RightPlayerEdge = Params.OwningActor->GetActorLocation() + (PlayerRightVector * HalfWidth);

	// Trig to make find hypotenuses of triangle formed by cone
	float SideLengths = CurrentRange / cos(HalfAngle);

	for (AActor* Actor : EdgeCases)
	{
		FHitResult OutHit;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Params.OwningActor);
		CollisionParams.AddIgnoredActors(ConfirmedHits);

		bool IsHitLeft = GetWorld()->LineTraceSingleByChannel(OutHit, LeftPlayerEdge, (LeftPlayerEdge + LeftConeDir * SideLengths), ECC_GameTraceChannel1, CollisionParams);

		if (IsHitLeft)
		{
			ConfirmedHits.Add(OutHit.GetActor());
			continue;
		}

		bool IsHitRight = GetWorld()->LineTraceSingleByChannel(OutHit, RightPlayerEdge, (RightPlayerEdge + RightConeDir * SideLengths), ECC_GameTraceChannel1, CollisionParams);

		if (IsHitRight)
		{
			ConfirmedHits.Add(OutHit.GetActor());
		}
	}
}

void UBoilAbilityTask::UpdateAttributes()
{
	// Angle
	CurrentAngle = FMath::Lerp(Params.BaseAngle, 0, CurrentConcentration);
	CurrentAngle = FMath::Clamp(CurrentAngle, 0, Params.BaseAngle);

	// Damage
	CurrentDamage = FMath::Lerp(Params.BaseDamage, Params.MaxDamage, CurrentConcentration);
	CurrentDamage = FMath::Clamp(CurrentDamage, Params.BaseDamage, Params.MaxDamage);

	// Range
	CurrentRange = FMath::Lerp(Params.BaseRange, Params.MaxRange, CurrentConcentration);
	CurrentRange = FMath::Clamp(CurrentRange, Params.BaseRange, Params.MaxRange);

	// Width
	CurrentWidth = FMath::Lerp(Params.BaseWidth, Params.MinWidth, CurrentConcentration);
	CurrentWidth = FMath::Clamp(CurrentWidth, Params.MinWidth, Params.BaseWidth);
	
	
}

void UBoilAbilityTask::UpdateVFXParameters()
{
	if (VFXComponent)
	{
		for (int i = 0; i < 4; i++)
		{
			FString ParameterString = FString("Boil") + FString::FromInt(i) + FString("_ConeAngle");
			FName ParameterName(*ParameterString);
			VFXComponent->SetFloatParameter(ParameterName, FMath::Lerp(StartConeAngles[i], EndConeAngles[i], CurrentConcentration));
		}

		for (int i = 0; i < 4; i++)
		{
			FString ParameterString = FString("Boil") + FString::FromInt(i) + FString("_Lifetime");
			FName ParameterName(*ParameterString);
			VFXComponent->SetFloatParameter(ParameterName, FMath::Lerp(StartLifetime[i], EndLifetime[i], CurrentConcentration));
		}

		for (int i = 0; i < 4; i++)
		{
			FString ParameterString = FString("Boil") + FString::FromInt(i) + FString("_SpawnRate");
			FName ParameterName(*ParameterString);
			VFXComponent->SetFloatParameter(ParameterName, FMath::Lerp(StartSpawnRate[i], EndSpawnRate[i], CurrentConcentration));
		}
	}

}

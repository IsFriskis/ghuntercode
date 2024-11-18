// TinyTavern


#include "AbilityTask_QuadCollisionCheck.h"

UAbilityTask_QuadCollisionCheck::UAbilityTask_QuadCollisionCheck(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = false;
}

UAbilityTask_QuadCollisionCheck* UAbilityTask_QuadCollisionCheck::QuadCollisionAbilityTask(UGameplayAbility* OwningAbility,
	FName TaskInstanceName, FVector ColliderStartLocation, float Extents, float AngleLimit, float ColliderStartWidth, TArray<TSubclassOf<AActor>> IgnoreActors, bool ShowHitbox)
{
	UAbilityTask_QuadCollisionCheck* QuadCollisionTask = NewAbilityTask<UAbilityTask_QuadCollisionCheck>(OwningAbility, TaskInstanceName);
	QuadCollisionTask->ColliderStartLocation = ColliderStartLocation;
	QuadCollisionTask->Extents = FMath::Abs(Extents);
	QuadCollisionTask->AngleLimit = FMath::Abs(AngleLimit);
	QuadCollisionTask->IgnoreActors = IgnoreActors;
	QuadCollisionTask->ShowHitbox = ShowHitbox;
	QuadCollisionTask->OwningActor = OwningAbility->GetAvatarActorFromActorInfo();
	QuadCollisionTask->ColliderStartWidth = FMath::Abs(ColliderStartWidth);

	return QuadCollisionTask;
}

void UAbilityTask_QuadCollisionCheck::Activate()
{
	Super::Activate();

	if (IsInsideBoxSweep())
	{
		FilterByAngle();
		HandleEdgeCases();
	}

	if (ConfirmedHits.Num() > 0)
	{
		for (AActor* Actor : ConfirmedHits)
		{
			OnHit.Broadcast(Actor);
		}
		OnCompleted.Broadcast(nullptr);
		EndTask();
	}
	else
	{
		OnCompleted.Broadcast(nullptr);
		EndTask();
	}
}

bool UAbilityTask_QuadCollisionCheck::IsInsideBoxSweep()
{
	FVector boxLocation = ColliderStartLocation + (OwningActor->GetActorForwardVector() * (Extents / 2));
	FCollisionShape collisionShape;
	FQuat playerQuaternion = OwningActor->GetActorRotation().Quaternion();

	/* Box is big so it can detect all objects no matter the angle
	- This is fine because the angle will be filtered out later */

	FVector3f boxExtents = FVector3f(Extents / 2, Extents / 2, 100.0f);
	collisionShape.SetBox(boxExtents);
	FCollisionQueryParams params;
	params.AddIgnoredActor(OwningActor);
	bool isHit = GetWorld()->SweepMultiByChannel(BoxHits, boxLocation, boxLocation, playerQuaternion, ECC_GameTraceChannel1, collisionShape, params);

	// Display Sweep box
	if (ShowHitbox)
	{
		DrawDebugBox(GetWorld(), boxLocation, static_cast<FVector>(boxExtents), playerQuaternion, FColor::Emerald, false, -1, 0, 5);
	}

	return isHit;
}

void UAbilityTask_QuadCollisionCheck::FilterByAngle()
{
	for (FHitResult& Hit : BoxHits)
	{
		float halfWidth = ColliderStartWidth / 2;

		FVector enemyPosition = Hit.GetActor()->GetActorLocation();

		FVector playerRightVector = OwningActor->GetActorRightVector();

		FVector leftPlayerEdge = ColliderStartLocation - (playerRightVector * halfWidth);
		FVector rightPlayerEdge = ColliderStartLocation + (playerRightVector * halfWidth);

		FVector hitToPlayerLeft = enemyPosition - leftPlayerEdge;
		FVector hitToPlayerRight = enemyPosition - rightPlayerEdge;

		hitToPlayerLeft.Z = 0;
		hitToPlayerLeft.Normalize();

		hitToPlayerRight.Z = 0;
		hitToPlayerRight.Normalize();

		FVector playerForwardVector = OwningActor->GetActorForwardVector();

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


		float halfAngle = AngleLimit / 2.0f;

		if (leftAngleDegrees >= -halfAngle && rightAngleDegrees <= halfAngle)
		{
			ConfirmedHits.Add(Hit.GetActor());
		}
		else
		{
			PotentialEdgeHits.Add(Hit.GetActor());
		}
	}
}

void UAbilityTask_QuadCollisionCheck::HandleEdgeCases()
{
	float HalfAngle = FMath::DegreesToRadians(AngleLimit / 2.0f);
	float HalfWidth = ColliderStartWidth / 2;
	FVector ForwardVector = OwningActor->GetActorForwardVector();
	FVector PlayerRightVector = OwningActor->GetActorRightVector();


	// Rotate left side vector
	float LeftXComp = (cos(-HalfAngle) * ForwardVector.X) - (sin(-HalfAngle) * ForwardVector.Y);
	float LeftYComp = (sin(-HalfAngle) * ForwardVector.X) + (cos(-HalfAngle) * ForwardVector.Y);
	FVector LeftConeDir = FVector(LeftXComp, LeftYComp, 0);
	LeftConeDir.Normalize();
	FVector LeftPlayerEdge = ColliderStartLocation - (PlayerRightVector * HalfWidth);
	

	// Rotate right side vector
	float RightXComp = (cos(HalfAngle) * ForwardVector.X) - (sin(HalfAngle) * ForwardVector.Y);
	float RightYComp = (sin(HalfAngle) * ForwardVector.X) + (cos(HalfAngle) * ForwardVector.Y);
	FVector RightConeDir = FVector(RightXComp, RightYComp, 0);
	RightConeDir.Normalize();
	FVector RightPlayerEdge = ColliderStartLocation + (PlayerRightVector * HalfWidth);

	// Trig to make find hypotenuses of triangle formed by cone
	float SideLengths = Extents / cos(HalfAngle);

	for (AActor* Actor : PotentialEdgeHits)
	{
		FHitResult OutHit;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwningActor);
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
	
	// Draw lines
	
	if (ShowHitbox)
	{
		// Left
		DrawDebugLine(GetWorld(), ColliderStartLocation - (PlayerRightVector * HalfWidth), (LeftPlayerEdge + LeftConeDir * SideLengths), FColor(255, 0, 0), false, -1, 0, 12.333);

		// Right
		DrawDebugLine(GetWorld(), ColliderStartLocation + (PlayerRightVector * HalfWidth), (RightPlayerEdge + RightConeDir * SideLengths), FColor(255, 0, 0), false, -1, 0, 12.333);
	}
	
	



}

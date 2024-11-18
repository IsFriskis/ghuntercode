// TinyTavern


#include "JamSlamAimTask.h"

#include "TinyTavernGame/TinyTavernUtils.h"
#include "TinyTavernGame/Player/PlayerCharacter.h"
#include "TinyTavernGame/Player/TTPlayerController.h"

UJamSlamAimTask::UJamSlamAimTask(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
	
}

TObjectPtr<UJamSlamAimTask> UJamSlamAimTask::ExecuteJamSlamAimTask(UGameplayAbility* OwningAbility,
	TObjectPtr<APlayerCharacter> _Player, float _PetrificationSpawnMaxRadius, float _PetrificationSpawnMinRadius,
	float _PetrificationAreaRadius, TObjectPtr<UAnimMontage> HoldAnim, TObjectPtr<UStaticMesh> _JamIndicatorMesh, float _AimSmoothFactor)
{
	TObjectPtr<UJamSlamAimTask> jamSlamTask = NewAbilityTask<UJamSlamAimTask>(OwningAbility);
	jamSlamTask.Get()->OwningAbility = Cast<UJamSlamAbility>(OwningAbility);
	jamSlamTask.Get()->Player = _Player;
	jamSlamTask.Get()->PetrificationAreaRadius = _PetrificationAreaRadius;
	jamSlamTask.Get()->PetrificationSpawnMaxRadius = _PetrificationSpawnMaxRadius;
	jamSlamTask.Get()->PetrificationSpawnMinRadius = _PetrificationSpawnMinRadius;
	jamSlamTask.Get()->Player.Get()->PlayAnimMontage(HoldAnim);
	jamSlamTask.Get()->PlayerController = Cast<ATTPlayerController>(_Player.Get()->GetController());
	jamSlamTask.Get()->Player.Get()->WeaponIndicatorStaticMesh.Get()->SetStaticMesh(_JamIndicatorMesh);
	jamSlamTask.Get()->LerpFactor = _AimSmoothFactor;
	jamSlamTask.Get()->Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(true);
	return jamSlamTask;
}


void UJamSlamAimTask::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeScale3D(FVector(PetrificationAreaRadius / 100, PetrificationAreaRadius / 100, 1));
	if(Cast<ATTPlayerController>(Player.Get()->Controller)->GetIsAimingPC())
	{
		//AIMING WITH MOUSE
		AimWithMouse();
	}
	else if(Cast<ATTPlayerController>(Player.Get()->Controller)->GetIsAimingController())
	{
		//AIMING WITH CONTROLLER
		AimWithController();
	}
	else
	{
		NotAiming();
	}
	PrevInput = PlayerController.Get()->AimInput;
}


void UJamSlamAimTask::AimWithMouse()
{
	float Distance = FVector::Dist(PlayerController.Get()->GetMouseProjectionToWorld(), Player->GetActorLocation());
	Distance = FMath::Clamp(Distance, PetrificationSpawnMinRadius, PetrificationSpawnMaxRadius);

	PlayerController.Get()->SetShowMouseCursor(false);
	//Area of petrification
	FVector actor = Player.Get()->GetActorLocation();
	FVector forward = Player.Get()->GetActorForwardVector() * Distance;
	FVector petrificationAreaLocation = actor + forward;
	
	OwningAbility.Get()->SetJamSpawnLocation(petrificationAreaLocation);
#if WITH_EDITOR
	//Aim circle
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation(), PetrificationSpawnMaxRadius, 8, FColor::Cyan, false, -1.0f);
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation(), PetrificationSpawnMinRadius, 8, FColor::Cyan, false, -1.0f);
	
	DrawDebugSphere(GetWorld(), petrificationAreaLocation, PetrificationAreaRadius, 8, FColor::Red, false, -1.0f);
#endif
	//Detect Floor
	FHitResult hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Player);
	FVector indicatorPosition;
	bool isHit = GetWorld()->LineTraceSingleByChannel(hit, petrificationAreaLocation + FVector(0,0,5000), petrificationAreaLocation,ECC_Visibility, QueryParams);
	if(isHit)
	{
		indicatorPosition = hit.Location;
	}
	else
	{
		indicatorPosition = petrificationAreaLocation;
	}
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(true);
	// indicatorPosition.Z = Player.Get()->WeaponIndicatorStaticMesh.Get()->GetComponentLocation().Z;
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldRotation(FRotator::ZeroRotator);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(indicatorPosition);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeScale3D(FVector(PetrificationAreaRadius / 100, PetrificationAreaRadius / 100, 1));
}

void UJamSlamAimTask::AimWithController()
{
	FVector2d aimInput = PlayerController.Get()->AimInput;

	
	if(aimInput.X == 0 && aimInput.Y == 0)
	{
		//Player not touching the aim button
		NewPos = PrevPos; 
	}
	else
	{
		NewPos = CalculateNewPosition(); // Implement this based on AimInput
	}
	// Adjust this value as needed for smoothness

	// Assuming PrevPos is a member variable of FVector type
	FVector InterpolatedPos = FMath::Lerp(PrevPos, NewPos, LerpFactor);

	// Update PrevPos for the next tick
	PrevPos = InterpolatedPos;
	
	//Area of petrification

	FVector petrificationAreaLocation = InterpolatedPos;
	
	if(FVector::Dist(Player.Get()->GetActorLocation(), petrificationAreaLocation) > PetrificationSpawnMaxRadius)
	{
		petrificationAreaLocation = Player.Get()->GetActorLocation() + (petrificationAreaLocation - Player.Get()->GetActorLocation()).GetSafeNormal() * PetrificationSpawnMaxRadius;
	}
	
	OwningAbility.Get()->SetJamSpawnLocation(petrificationAreaLocation);
	
#if WITH_EDITOR
	//Aim circle
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation(), PetrificationSpawnMaxRadius, 8, FColor::Cyan, false, -1.0f);
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation(), PetrificationSpawnMinRadius, 8, FColor::Cyan, false, -1.0f);
	DrawDebugSphere(GetWorld(), petrificationAreaLocation, PetrificationAreaRadius, 8, FColor::Red, false, -1.0f);
#endif
	//Detect Floor
	FHitResult hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Player);
	FVector indicatorPosition;
	bool isHit = GetWorld()->LineTraceSingleByChannel(hit, petrificationAreaLocation + FVector(0,0,5000), petrificationAreaLocation,ECC_Visibility, QueryParams);
	if(isHit)
	{
		indicatorPosition = hit.Location;
	}
	else
	{
		indicatorPosition = petrificationAreaLocation;
	}
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetVisibility(true);
	// indicatorPosition.Z = Player.Get()->WeaponIndicatorStaticMesh.Get()->GetComponentLocation().Z;
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldRotation(FRotator::ZeroRotator);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(indicatorPosition);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetRelativeScale3D(FVector(PetrificationAreaRadius / 100, PetrificationAreaRadius / 100, 1));
}

void UJamSlamAimTask::NotAiming()
{
	float Distance = PetrificationSpawnMaxRadius * PrevInput.Size();
	Distance = FMath::Clamp(Distance, PetrificationSpawnMinRadius, PetrificationSpawnMaxRadius);

	//Area of petrification
	FVector actor = Player.Get()->GetActorLocation();
	FVector forward = Player.Get()->GetActorForwardVector() * Distance;
	FVector petrificationAreaLocation = actor + forward;

	OwningAbility.Get()->SetJamSpawnLocation(actor);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldRotation(FRotator::ZeroRotator);
	Player.Get()->WeaponIndicatorStaticMesh.Get()->SetWorldLocation(actor);

	//Reset Position
	PrevPos = Player.Get()->GetActorLocation();
#if WITH_EDITOR
	//Aim circle
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation(), PetrificationSpawnMaxRadius, 8, FColor::Cyan, false, -1.0f);
	DrawDebugSphere(GetWorld(), Player.Get()->GetActorLocation(), PetrificationSpawnMinRadius, 8, FColor::Cyan, false, -1.0f);
	DrawDebugSphere(GetWorld(), petrificationAreaLocation, PetrificationAreaRadius, 8, FColor::Red, false, -1.0f);
#endif
}

FVector UJamSlamAimTask::CalculateNewPosition()
{
	// Get the AimInput vector from the controller
	FVector2D AimInput = PlayerController.Get()->AimInput;
	//AimInput.X -= 0.3;
	// Normalize the AimInput to get a direction vector
	FVector2D AimDirection = AimInput.GetSafeNormal();

	// Calculate the distance for the aim based on the controller's input magnitude
	float Distance = PetrificationSpawnMaxRadius * AimInput.Size();
	Distance = FMath::Clamp(Distance, PetrificationSpawnMinRadius, PetrificationSpawnMaxRadius);

	// Convert the 2D aim direction to a 3D vector, assuming no vertical movement (Z = 0)
	FVector AimDirection3D(AimDirection.X, AimDirection.Y, 0.0f);

	// Rotate the aim direction by 45 degrees to the left
	FRotator Rotation = FRotator(0.0f, -45.0f, 0.0f);
	FVector RotatedAimDirection3D = Rotation.RotateVector(AimDirection3D);

	// Scale the direction by the distance
	FVector ScaledAimDirection = RotatedAimDirection3D * Distance;

	// Calculate the new position by adding the scaled direction to the player's current location
	FVector NewPosition = Player->GetActorLocation() + ScaledAimDirection;

	return NewPosition;
}

// TinyTavern


#include "GuardianWall.h"


// Sets default values
AGuardianWall::AGuardianWall()
{
	PrimaryActorTick.bCanEverTick = true;
	WallStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("WallStaticMesh");
	RootComponent = WallStaticMesh;
}

void AGuardianWall::HideWallsAgain(float DeltaSeconds)
{
	isClosing = true;
	GuardianWallDoorBlendDown.Update(DeltaSeconds);
	FVector NewPosition = FMath::Lerp(GuardianWallTargetPosition, GuardianWallInitialPosition,
									  GuardianWallDoorBlendDown.GetAlpha());
	SetActorLocation(NewPosition);
}

// Called when the game starts or when spawned
void AGuardianWall::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	WallStaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	GuardianWallInitialPosition = GetActorLocation();
	GuardianWallTargetPosition = GuardianWallInitialPosition + FVector(0, 0, WallTargetHeight);
	GuardianWallDoorBlendDown.SetBlendTime(WallBlendTimeDown);
	GuardianWallDoorBlend.SetBlendTime(WallBlendTimeUp);
}

void AGuardianWall::Tick(float DeltaSeconds)
{
	if (!IsHidden() && isClosing == false)
	{
		GuardianWallDoorBlend.Update(DeltaSeconds);
		FVector NewPosition = FMath::Lerp(GuardianWallInitialPosition, GuardianWallTargetPosition,
		                                  GuardianWallDoorBlend.GetAlpha());
		SetActorLocation(NewPosition);
	}
	else if(GetActorLocation().Equals(GuardianWallInitialPosition) && isClosing)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		WallStaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		WallStaticMesh->SetCanEverAffectNavigation(false);
		isClosing = false;
	}
}
